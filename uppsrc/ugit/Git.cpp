#include "Git.h"

using namespace Upp;

#define IMAGECLASS GitImg
#define IMAGEFILE  <ugit/Git.iml>
#include <Draw/iml_source.h>

bool CheckGit()
{
	String h;
	if (Sys("git", h) >= 0)
		return true;
	return false;
}

bool IsGitDir(const String& p)
{
	if (GetGitRoot(p).IsEmpty())
		return false;
	else
		return true;
}

bool IsGitFile(const String& p)
{
	bool result = false;
	String folder = GetGitRoot(p);
	if (DirectoryExists(folder)) {
		String command = "git --no-pager -C " << folder << " ls-files --error-unmatch " << p;
		String out;
		if (Sys(command, out) == 0)
			result = true;
	}
	return result;
}

String GetGitRoot(const String& p)
{
	String path;

	if (IsNull(p))
		return (path);
	if (DirectoryExists(AppendFileName(p, ".git")))
		return (p);

	path = p;
	String path0;
	while (path != path0) {
		path0 = path;
		DirectoryUp(path);
		if (DirectoryExists(AppendFileName(path, ".git")))
			break;
	}
	return (path);
}

Git& Git::CmdExecute(const String& cmd)
{
	String folder = GetGitDir();
	output.Clear();
	errorcode = 0;

	if (CheckGit() && DirectoryExists(folder)) {
		Progress pi("Executing git command");
		String command = "git --no-pager -C " << folder << " " << cmd;
		LocalProcess p;

		if(!p.Start(command)) {
			Exclamation("[= Execution of the git command failed!&&" + DeQtf(command) + "]");
		} else {
			String tmp;
			while(p.IsRunning()) {
				String h = p.Get();
				if(IsNull(h))
					Sleep(1);
				else
					tmp.Cat(h);
				pi.Step();

				if (pi.Canceled())
					p.Kill();
			}
			errorcode = p.GetExitCode();

			// read the rest of output
			for(;;) {
				String h = p.Get();
				if(h.IsVoid())
					break;
				tmp.Cat(h);
			}
			output = Split(tmp, CharFilterCrLf);
			pi.Close();
		}
	}
	return *this;
}

Git& Git::CmdExecute(const String& cmd, const Vector<String>& parameters)
{
	String command = cmd;
	for (int i = 0; i < parameters.GetCount(); ++i) {
		command = command << " " << parameters[i];
	}
	return (CmdExecute(command));
}

Git& Git::Push(const String& parameters)
{
	String command = "push " << parameters << " -v";
	Vector<String> tmp = CmdExecute(command).GetOutput();
	command = "push -v --tags";
	tmp.Append(CmdExecute(command).GetOutput());
	SetOutput(tmp);
	return *this;
}

Git& Git::Commit(const Vector<String>& parameters, const String& msgpath)
{
	String command = "commit";
	command.Cat() << " -F \"" << msgpath << "\"";
	CmdExecute(command, parameters);
	FileDelete (msgpath);
	return *this;
}

Git& Git::AddToGitignore(const Vector<String>& filelist)
{
	String gitignore = AppendFileName(GetGitDir(), ".gitignore");
	bool exist = FileExists(gitignore);

	FileAppend f(gitignore);
	if (!f.IsOpen()) {
		output.Add("Open .gitignore has failed.");
		return *this;
	}

	if (!exist) {
		f << "# Lines that start with '#' are comments." << "\n";
		f << "# For a project mostly in C, the following would be a good set of" << "\n";
		f << "# exclude patterns (uncomment them if you want to use them):" << "\n";
		f << "# *.[oa]" << "\n";
		f << "# *~" << "\n";
	}

	for (int i = 0; i < filelist.GetCount(); ++i) {
		output.Add(Format("File added to .gitignore: %s", filelist[i]));
		f << filelist[i] << "\n";
	}

	f.Close();
	return *this;
}

Git& Git::BranchCreate(const String& branchname, const String& startpoint, bool checkout)
{
	String command = "branch " << branchname;
	if (checkout)
		command = "checkout -b " << branchname;

	if (startpoint.GetCount() > 0)
		command.Cat() << " " << startpoint;

	CmdExecute(command);
	return *this;
}

Git& Git::BranchSetMerge(const String& localbranch, const String& upstream)
{
	String stream;
	if (upstream.Find("remotes") == 0)
		stream = upstream;
	else
		stream = "remotes/" << upstream;

	String command = "branch " << localbranch << " -u " << stream;
	CmdExecute(command);
	return *this;
}

String Git::GetBranchStatus(const String& localbranch, const String& remotebranch)
{
	if (remotebranch.GetCount() == 0)
		return(Format("Branch '%s' doesn't have upstream configuration", localbranch));

	String command = "rev-list " << TrimBoth(localbranch) << ".." << TrimBoth(remotebranch);
	Vector<String> local = CmdExecute(command).GetOutput();
	int localerror = GetCmdErrorCode();
	command = "rev-list " << TrimBoth(remotebranch) << ".." << TrimBoth(localbranch);
	Vector<String> remote = CmdExecute(command).GetOutput();
	int remoteerror = GetCmdErrorCode();

	String status = Format("Branch '%s' is ", TrimBoth(localbranch));
	if (localerror || remoteerror) {
		status = "Remote branch status failed!";
	} else if (remote.IsEmpty() && local.IsEmpty()) {
		status.Cat() << "up-to-date with '" << TrimBoth(remotebranch) << "'";
	} else if (remote.IsEmpty() && local.GetCount() > 0) {
		status.Cat() << "behind of '" << TrimBoth(remotebranch) << "' by " << local.GetCount() << " commit";
		if (local.GetCount() > 1)
			status.Cat() << "s";
	} else if (local.IsEmpty() && remote.GetCount() > 0) {
		status.Cat() << "ahead of '" << TrimBoth(remotebranch) << "' by " << remote.GetCount() << " commit";
		if (remote.GetCount() > 1)
			status.Cat() << "s";
	} else {
		status.Cat() << AsString(remote.GetCount()) << " commit";
		if (remote.GetCount() > 1)
			status.Cat() << "s";
		status.Cat() << " ahead, " << AsString(local.GetCount()) << " commit";
		if (local.GetCount() > 1)
			status.Cat() << "s";
		status.Cat() << " behind of '" << TrimBoth(remotebranch) << "'";
	}

	return (status);
}

Git& Git::GetBranchStatusAll()
{
	Fetch("");
	Vector<String> tmp;
	Vector<String> branches = GetBranchAll().GetOutput();
	for (int i = 0; i < branches.GetCount(); ++i) {
		String localbranch = TrimBoth(branches[i]);
		if (localbranch.Find("* ") == 0)
			localbranch = localbranch.Mid(2);
		String remote = GetBranchRemote(localbranch);
		String merge = GetFileName(GetBranchMerge(localbranch));
		String remotebranch = remote + "/" + merge;
		if (remote.GetCount() == 0 || merge.GetCount() == 0)
			remotebranch.Clear();
		tmp.Add(GetBranchStatus(localbranch, remotebranch));
	}
	SetOutput(tmp);
	return *this;
}

Git& Git::GetBranchLocal(String& branchname)
{
	branchname.Clear();
	Vector<String> output = CmdExecute("rev-parse --abbrev-ref HEAD").GetOutput();
	if (!GetCmdErrorCode() && output.GetCount() > 0)
		branchname = output[0];
	return *this;
}

Git& Git::GetBranchLocalRemote(String& remote)
{
	remote.Clear();
	Vector<String> output = CmdExecute("rev-parse --abbrev-ref --symbolic-full-name @{u}").GetOutput();
	if (!GetCmdErrorCode() && (output.GetCount() > 0))
		remote = output[0];
	return *this;
}

Git& Git::GetBranchRemote(const String& branchname, String& remote)
{
	remote.Clear();
	if (branchname.GetCount() > 0) {
		Vector<String> output = CmdExecute("config --local branch." << TrimBoth(branchname) << ".remote").GetOutput();
		if (!GetCmdErrorCode() && output.GetCount() > 0)
			remote = output[0];
	}
	return *this;
}

Git& Git::GetBranchMerge(const String& branchname, String& mergebranch)
{
	mergebranch.Clear();
	if (branchname.GetCount() > 0) {
		Vector<String> output = CmdExecute("config --local branch." << TrimBoth(branchname) << ".merge").GetOutput();
		if (!GetCmdErrorCode() && output.GetCount() > 0)
			mergebranch = output[0];
	}
	return *this;
}

Git& Git::GetBranchDescription(const String& branchname, String& description)
{
	description.Clear();
	if (branchname.GetCharCount() > 0) {
		Vector<String> output = CmdExecute("config --local branch." << TrimBoth(branchname) << ".description").GetOutput();
		if (!GetCmdErrorCode() && output.GetCount() > 0)
			description = output[0];
	}
	return *this;
}

Git& Git::GetRemoteUrl(const String& remotename, String& url)
{
	url.Clear();
	if (remotename.GetCount() > 0) {
		Vector<String> output = CmdExecute("config --local remote." << TrimBoth(remotename) << ".url").GetOutput();
		if (!GetCmdErrorCode() && output.GetCount() > 0)
			url = output[0];
	}
	return *this;
}

Git& Git::GetGitConfig(bool local)
{
	if (local)
		CmdExecute("config -l --local");
	else
		CmdExecute("config -l --global");
	return *this;
}

Git& Git::GetGitConfig(bool local, const String& name, String& value)
{
	String command = "config --global";
	if (local)
		command = "config --local";
	command.Cat() << " --get " << name;

	Vector<String> output = CmdExecute(command).GetOutput();
	if (!GetCmdErrorCode() && output.GetCount() > 0)
		value = output[0];
	return *this;
}

Git& Git::SetGitConfig(bool local, char type, const String& name, const String& value)
{
	String command = "config";
	String parameters;

	switch (type) {
		// Add
		case 'A': parameters.Cat() << "--add " << name << " \"" << value << "\"";
			break;
		// Unset (Delete)
		case 'U': parameters.Cat() << "--unset " << name;
			break;
		// Set
		default : parameters.Cat() << name << " \"" << value << "\"";
			break;
	}

	if (local) {
		command.Cat() << " --local " << parameters;
	} else {
		command.Cat() << " --global " << parameters;
	}

	CmdExecute(command);
	return *this;
}

Git& Git::GetDiff(int type)
{
	String command = "diff --no-color";

	switch (type) {
		case DIFF_CACHED: command.Cat() << " --cached";
			break;
		case DIFF_HEAD:	command.Cat() << " HEAD";
			break;
		default:
			break;
	}

	CmdExecute(command);
	return *this;
}

Git& Git::GetDiff(const String& hash, bool files)
{
	String command = "diff --no-color ";
	if (files)
		command.Cat() << " --name-status ";

	String h = TrimBoth(hash);
	if (h.Compare("HEAD") == 0) {
		Vector<String> tmp = CmdExecute("rev-parse HEAD").GetOutput();
		if (!GetCmdErrorCode() && tmp.GetCount() > 0)
			h = tmp[0];
	}

	// 4b825dc642cb6eb9a060e54bf8d69288fbee4904 is the id of the "empty tree" in Git
	// and it's always available in every repository. We have to use this id to get
	// diff of the first commit in the repository
	Vector<String> tmp = CmdExecute("rev-list --max-parents=0 HEAD").GetOutput();
	if (!GetCmdErrorCode() && tmp.GetCount() > 0 && tmp[0].Compare(h) == 0)
		command.Cat() << "4b825dc642cb6eb9a060e54bf8d69288fbee4904 HEAD";
	else
		command.Cat() << h << "^!";
	CmdExecute(command);
	return *this;
}

bool Git::IsPatchApplyPending()
{
	Vector<String> tmp = CmdExecute("rev-parse --git-path rebase-apply").GetOutput();
	String dirname;
	if (!GetCmdErrorCode() && tmp.GetCount() > 0)
		dirname = AppendFileName(GetGitDir(), tmp[0]);
	return (DirectoryExists(dirname));
}

Git& Git::GetMergeDiff(const String& mergefrom, const String& mergeto, bool files)
{
	String command = "diff --no-color " << TrimBoth(mergefrom) << "..." << TrimBoth(mergeto);
	if (files)
		command.Cat() << " --name-only";
	CmdExecute(command);
	return *this;
}

Git& Git::Merge(const String& mergefrom, bool noff)
{
	String command = "merge";
	String parameters = " -v --stat --no-commit";
	if (noff)
		parameters.Cat() << " --no-ff";
	command.Cat() << " " << mergefrom << " " << parameters;
	CmdExecute(command);
	return *this;
}

Git& Git::Stash(const String& message)
{
	String command = "stash";
	if (message.GetCount() > 0)
		command.Cat() << " save \"" << message << "\"";
	CmdExecute(command);
	return *this;
}

Git& Git::Reset(const Vector<String>& filelist, bool head)
{
	String command = "reset";
	if (head)
		command.Cat() << " HEAD";
	CmdExecute(command, filelist);
	return *this;
}

Git& Git::GetLog(bool firstparent)
{
	String command = "log --graph --pretty=format:\">< %H >< %d >< %s\" --no-color";
	if (firstparent)
		command.Cat() << " --first-parent";
	CmdExecute(command);
	return *this;
}

Git& Git::TagAdd(const String& hash, const String& tagname, const String& message, bool force)
{
	if (message.GetCount() > 0) {
		String command = "tag ";
		String parameters = "-a -m \"" << message << "\" ";
		if (force)
			parameters.Cat() << "-f ";
		command.Cat() <<  parameters << tagname << " " << hash;
		CmdExecute(command);
	}
	return *this;
}

char Git::GetConfigChangeType(String& oldvalue, String& newvalue)
{
	char type = 'S';
	if (oldvalue.IsEmpty())
		type = 'A';
	if (newvalue.IsEmpty())
		type = 'U';
	return (type);
}

Git& Git::ProcessStashApply()
{
	Vector<String> stashlist = StashList().GetOutput();

	if (stashlist.GetCount() > 0) {
		WithGitCmdOutputLayout<TopWindow> stashApply;
		CtrlLayout(stashApply, "Apply stash changes");

		stashApply.Sizeable().Zoomable();
		stashApply.Acceptor(stashApply.ok, IDOK);
		stashApply.ok.Ok().SetLabel("Apply");
		stashApply.Rejector(stashApply.close, IDCANCEL);
		stashApply.close.Cancel().SetLabel("Cancel");
		stashApply.WhenClose = stashApply.Rejector(IDCANCEL);

		stashApply.cmd.AddColumn("Stash name").HeaderTab().Fixed(100);
		stashApply.cmd.AddColumn("Stash description");
		stashApply.cmd.NoHeader();
		stashApply.cmd.NoWantFocus();

		for (int i = 0; i < stashlist.GetCount(); ++i) {
			Vector<String> tmp = Split(stashlist[i], "}:");
			stashApply.cmd.Add(tmp[0] + "}", tmp[1]);
		}
		stashApply.cmd.GoBegin();

		int c = stashApply.RunAppModal();
		if (c == IDOK) {
			int row = stashApply.cmd.GetCursor();
			if (row >= 0) {
				String stashname = stashApply.cmd.Get(row, 0);
				StashApply(stashname);
			}
		}
	}
	return *this;
}

Git& Git::ProcessBranchAdd(const String& startpoint)
{
	Vector<String> list = GetBranchAll().GetOutput();
	if (list.IsEmpty()) {
		Exclamation("Can't add new branch to a freshly created repository.");
		return *this;
	}

	WithGitConfigBranchAddLayout<TopWindow> branchAdd;
	CtrlLayout(branchAdd, "Add new branch");

	branchAdd.Acceptor(branchAdd.ok, IDOK);
	branchAdd.ok.Ok();
	branchAdd.Rejector(branchAdd.cancel, IDCANCEL);
	branchAdd.cancel.Cancel();
	branchAdd.WhenClose = branchAdd.Rejector(IDCANCEL);
	branchAdd.param1text.SetText("Branch name:");
	branchAdd.param2text.SetText("Description:");

	branchAdd.id.Clear();

	list.Append(GetBranchAll(false).GetOutput());
	for (int i = 0; i < list.GetCount(); ++i) {
		String key = list[i].Find("* ") == 0 ? list[i].Mid(2) : list[i];
		if (list[i].Find("->") == -1)
			branchAdd.id.Add(TrimBoth(key), TrimBoth(list[i]));
	}

	branchAdd.id.Add("---");

	list = GetHashLog().GetOutput();
	for (int i = 0; i < list.GetCount(); ++i) {
		Vector<String> tmp = Split(list[i], " >< ");
		if (tmp.GetCount() > 1) {
			AttrText val(Format("<%s> | %s", tmp[0].Left(GIT_HASH_SIZE), tmp[1]));
			val.SetFont(Monospace().Height(StdFont().GetHeight() - 2));
			branchAdd.id.Add(tmp[0], val);
		}
	}

	branchAdd.id = startpoint;

	int c = branchAdd.RunAppModal();
	if (c == IDOK) {
		String branch = TrimBoth(branchAdd.param1.GetData().ToString());
		if (branchAdd.param1.IsModified() && branch.GetCount() > 0) {
			String id;
			int index = branchAdd.id.GetIndex();
			if (index >= 0)
				id = branchAdd.id.GetKey(index);

			if (id.Compare("---") == 0) {
				Exclamation("This start point can't be used!");
				return *this;
			}
			BranchCreate(branch, id, ~branchAdd.opt);
			if (branchAdd.param2.IsModified()) {
				String name = "branch." << branch << ".description";
				SetGitConfig(true, 'A', name, ~branchAdd.param2);
			}
		} else {
			Exclamation("Branch name can't be empty!");
		}
	}
	return *this;
}

void Git::ProcessUpdateStatus(const Vector<String>& gitdirs)
{
	Vector<String> tmp;
	String separator = "Repository: ";

	if (gitdirs.GetCount() > 0) {
		Git git;
		for (int i = 0; i < gitdirs.GetCount(); ++i) {
			if (IsGitDir(gitdirs[i])) {
				git.SetGitDir(gitdirs[i]);
				tmp.Add(separator + gitdirs[i]);
				git.GetBranchStatusAll();
				tmp.Append(git.GetOutput());
			}
		}
	} else {
		if (IsGitDir(GetGitDir())) {
			tmp.Add(separator + GetGitDir());
			GetBranchStatusAll();
			tmp.Append(GetOutput());
		}
	}

	WithGitCmdOutputLayout<TopWindow> updateStatus;
	CtrlLayout(updateStatus, "GIT update status");

	updateStatus.Sizeable().Zoomable();
	updateStatus.ok.Ok().Hide();
	updateStatus.Rejector(updateStatus.close, IDCANCEL);
	updateStatus.WhenClose = updateStatus.Rejector(IDCANCEL);

	updateStatus.cmd.AddColumn("Repository");
	updateStatus.cmd.AddColumn("Branch status");
	updateStatus.cmd.NoWantFocus();

	String repository;
	for (int i = 0; i < tmp.GetCount(); ++i) {
		if (tmp[i].Find(separator) == 0) {
			repository = tmp[i].Mid(separator.GetLength());
			continue;
		}
		updateStatus.cmd.Add(repository, tmp[i]);
	}

	if (updateStatus.cmd.GetCount() > 0)
		updateStatus.RunAppModal();
	else
		Exclamation("GIT update status is not available!");
}

void Git::ProcessCommandOutput()
{
	if (output.GetCount() > 0) {
		WithGitCmdOutputLayout<TopWindow> commandOutput;
		CtrlLayout(commandOutput, "GIT command output");
		commandOutput.ok.Hide();

		commandOutput.Sizeable().Zoomable();
		commandOutput.Rejector(commandOutput.close, IDCANCEL);
		commandOutput.close.Cancel();
		commandOutput.WhenClose = commandOutput.Rejector(IDCANCEL);

		commandOutput.cmd.AddColumn("Command output");
		commandOutput.cmd.NoGrid().NoHeader();
		commandOutput.cmd.NoWantFocus();

		commandOutput.cmd.Clear();
		for (int i = 0; i < output.GetCount(); ++i) {
			commandOutput.cmd.Add(output[i]);
		}
		commandOutput.RunAppModal();
		output.Clear();
	}
}

int Git::EditCommitMessage(const String& message, const Vector<String>& parameters, String& msgpath)
{
	msgpath.Clear();
	String msg = message;

	if(message.GetCount() > 0) // NOLINT
		msg.Cat() << "\n";

	msg.Cat() << "\n"
		"# Please enter the commit message for your changes. Lines starting\n"
		"# with '#' will be ignored, and an empty message aborts the commit.\n#\n";

	int errorcode = CmdExecute("commit --dry-run", parameters).GetCmdErrorCode();

	if (!errorcode) {
		bool first = true;
		for (int i = 0; i < output.GetCount(); ++i) {
			if (output[i].Find("(") >= 0) {
				if (first) {
					first = false;
					msg.Cat() << "# " << output[i] <<"\n";
				}
				continue;
			}
			if ((!first && !IsSpace(output[i][0])) || output[i].Find("On branch") == 0)
				msg.Cat() << "#\n";
			msg.Cat() << "# " << output[i] << "\n";
		}

		WithGitCommitMessageLayout<TopWindow> c;
		CtrlLayout(c, "GIT commit message");
		c.Acceptor(c.ok, IDOK);
		c.Rejector(c.close, IDCANCEL);

		c.ok.SetLabel("Commit");
		c.close.SetLabel("Cancel");

		c.message <<= msg;

		errorcode = -1;
		for (;;) {
			int id = c.Run();

			if (id == IDOK) {
				msgpath = GetTempFileName();
				FileOut fstream(msgpath);
				for (int i = 0; i < c.message.GetLineCount(); ++i) {
					String tmp = c.message.GetEncodedLine(i) + "\n";
					if ((tmp.GetCount() > 0) && (tmp[0] != '#'))
						fstream.Put(tmp);
				}
				fstream.Flush();
				errorcode = 0;
				break;
			}
			if (id == IDCANCEL) {
				break;
			}
		}
	}

	return (errorcode);
}

// vim: ts=4
