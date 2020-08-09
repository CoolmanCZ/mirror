#ifndef _ugit_Git_h_
#define _ugit_Git_h_

#include <Core/Core.h>
#include <CtrlLib/CtrlLib.h>

#define IMAGECLASS GitImg
#define IMAGEFILE <ugit/Git.iml>

#include <Draw/iml_header.h>

namespace Upp {
#define LAYOUTFILE <ugit/Git.lay>
#include <CtrlCore/lay.h>
}

bool CheckGit();
bool IsGitDir(const Upp::String& p);
bool IsGitFile(const Upp::String& p);
Upp::String GetGitRoot(const Upp::String& p);

#define DIFF_TREE	0
#define DIFF_CACHED	1
#define DIFF_HEAD	2

#define GIT_HASH_SIZE 9

struct Git {
  private:
	Upp::String gitdir;
	int errorcode = 0;
	Upp::Vector<Upp::String> output;

  public:
	Git& SetGitDir(const Upp::String& path) {
		gitdir = path;
		return *this;
	}
	Upp::String GetGitDir() { return (gitdir); }

	Git& SetOutput(Upp::Vector<Upp::String>& out) {
		output = Upp::pick(out);
		return *this;
	}
	Upp::Vector<Upp::String> GetOutput() { return Upp::pick(output); }
	bool IsOutput() { return (output.GetCount() > 0); }

	int GetCmdErrorCode() { return errorcode; }

	Git& CmdExecute(const Upp::String& cmd);
	Git& CmdExecute(const Upp::String& cmd, const Upp::Vector<Upp::String>& parameters);

	Git& InitRepository() {
		CmdExecute("init");
		return *this;
	}

	Git& GetStatus() {
		CmdExecute("status --porcelain");
		return *this;
	}
	Git& GetStatusVerbose() {
		CmdExecute("status");
		return *this;
	}
	Git& GetStatusTracked() {
		CmdExecute("status --porcelain --untracked-files=no");
		return *this;
	}

	Git& Fetch(const Upp::String& parameters) {
		CmdExecute("fetch " << parameters << " -v --tags");
		return *this;
	}
	Git& FetchAll() {
		Fetch("--all");
		return *this;
	}

	Git& Push(const Upp::String& parameters);
	Git& PushAll() {
		Push("--all");
		return *this;
	}

	Git& Commit(const Upp::Vector<Upp::String>& parameters, const Upp::String& msgpath);
	bool IsCommitList() {
		int error = GetStatusTracked().GetCmdErrorCode();
		return (error == 0 && GetOutput().GetCount() > 0);
	}

	Git& Unstage(const Upp::Vector<Upp::String>& filelist) {
		CmdExecute("rm --cached", filelist);
		return *this;
	}
	Git& CancelCheckout(const Upp::Vector<Upp::String>& filelist) {
		CmdExecute("checkout --", filelist);
		return *this;
	}
	Git& AddToRepository(const Upp::Vector<Upp::String>& filelist) {
		CmdExecute("add", filelist);
		return *this;
	}
	Git& AddToRepository(const Upp::String& filepath) {
		Upp::Vector<Upp::String> tmp;
		tmp.Add(filepath);
		AddToRepository(tmp);
		return *this;
	}
	Git& AddToGitignore(const Upp::Vector<Upp::String>& filelist);
	Git& AddToGitignore(const Upp::String& filepath) {
		Upp::Vector<Upp::String> tmp;
		tmp.Add(filepath);
		AddToGitignore(tmp);
		return *this;
	}
	Git& DelFromRepository(const Upp::Vector<Upp::String>& filelist) {
		CmdExecute("rm -f -q -r", filelist);
		return *this;
	}
	Git& DelFromRepository(const Upp::String& filepath) {
		Upp::Vector<Upp::String> tmp;
		tmp.Add(filepath);
		DelFromRepository(tmp);
		return *this;
	}
	Git& RenameInRepository(const Upp::String& oldname, const Upp::String& newname) {
		CmdExecute("mv " << oldname << " " << newname);
		return *this;
	}

	Git& BranchCreate(const Upp::String& branchname, const Upp::String& startpoint, bool checkout);
	Git& BranchDelete(const Upp::String& branchname) {
		CmdExecute("branch -d " << branchname);
		return *this;
	}
	Git& BranchCheckout(const Upp::String& branchname) {
		CmdExecute("checkout " << branchname);
		return *this;
	}
	Git& BranchRename(const Upp::String& currentname, const Upp::String& newname) {
		CmdExecute("branch -m " << currentname << " " << newname);
		return *this;
	}
	Git& BranchSetMerge(const Upp::String& localbranch, const Upp::String& upstream);
	Git& BranchFetchRemote(const Upp::String& remote, const Upp::String& remotebranch,
						   const Upp::String& localbranch) {
		CmdExecute("fetch " << remote << " " << TrimBoth(remotebranch) << ":"
							<< TrimBoth(localbranch));
		return *this;
	}
	Git& BranchPushRemote(const Upp::String& remote, const Upp::String& branchname) {
		CmdExecute("push -u " << remote << " " << branchname);
		return *this;
	}
	Git& BranchDeleteRemote(const Upp::String& remote, const Upp::String& branchname) {
		CmdExecute("push " << remote << " --delete " << branchname);
		return *this;
	}
	Git& GetBranchAll(bool local = true) {
		if (local) {
			CmdExecute("branch");
		} else {
			CmdExecute("branch -r");
		}
		return *this;
	}
	Upp::String GetBranchStatus(const Upp::String& localbranch, const Upp::String& remotebranch);
	Git& GetBranchStatusAll();
	Git& GetBranchLocal(Upp::String& branchname);
	Git& GetBranchLocalRemote(Upp::String& remote);
	Git& GetBranchRemote(const Upp::String& branchname, Upp::String& remote);
	Git& GetBranchMerge(const Upp::String& branchname, Upp::String& mergebranch);
	Git& GetBranchDescription(const Upp::String& branchname, Upp::String& description);
	Upp::String GetBranchLocal() {
		Upp::String val;
		GetBranchLocal(val);
		return (val);
	}
	Upp::String GetBranchLocalRemote() {
		Upp::String val;
		GetBranchLocalRemote(val);
		return (val);
	}
	Upp::String GetBranchRemote(const Upp::String& branchname) {
		Upp::String val;
		GetBranchRemote(branchname, val);
		return (val);
	}
	Upp::String GetBranchMerge(const Upp::String& branchname) {
		Upp::String val;
		GetBranchMerge(branchname, val);
		return (val);
	}
	Upp::String GetBranchDescription(const Upp::String& branchname) {
		Upp::String val;
		GetBranchDescription(branchname, val);
		return (val);
	};

	Git& RemoteCreate(const Upp::String& remotename, const Upp::String& remoteurl) {
		CmdExecute("remote add " << remotename << " " << remoteurl);
		return *this;
	}
	Git& RemoteDelete(const Upp::String& remotename) {
		CmdExecute("remote rm " << remotename);
		return *this;
	}
	Git& RemoteRename(const Upp::String& currentname, const Upp::String& newname) {
		CmdExecute("remote rename " << currentname << " " << newname);
		return *this;
	}
	Git& GetRemotes() {
		CmdExecute("remote");
		return *this;
	}
	Git& GetRemoteUrl(const Upp::String& remotename, Upp::String& url);
	Upp::String GetRemoteUrl(const Upp::String& remotename) {
		Upp::String val;
		GetRemoteUrl(remotename, val);
		return (val);
	}

	Git& GetGitConfig(bool local = true);
	Git& GetGitConfig(bool local, const Upp::String& name, Upp::String& value);
	Upp::String GetGitConfig(bool local, const Upp::String& name) {
		Upp::String value;
		GetGitConfig(local, name, value);
		return value;
	}
	Git& SetGitConfig(bool local, char type, const Upp::String& name, const Upp::String& value);

	Git& GetDiff(int type);
	Git& GetDiff(const Upp::String& oldversion, const Upp::String& newversion) {
		CmdExecute("diff --no-color " << oldversion << " " << newversion);
		return *this;
	}
	Git& GetDiff(const Upp::String& hash, bool files = false);
	Git& GetPatch(const Upp::String& hash) {
		CmdExecute("format-patch --stdout -1 " << hash);
		return *this;
	}
	Git& PatchApply(const Upp::String& filepath) {
		CmdExecute("am " << filepath);
		return *this;
	}
	Git& PatchAbort() {
		CmdExecute("am --abort");
		return *this;
	}
	bool IsPatchApplyPending();

	Git& GetMergeDiff(const Upp::String& mergefrom, const Upp::String& mergeto, bool files = false);
	Git& Merge(const Upp::String& mergefrom, bool noff);
	bool IsMergePending() {
		// returns a 128 code if a merge is in progress,
		// returns a 1 code for an error or 0 otherwise.
		return (CmdExecute("merge HEAD").GetCmdErrorCode() == 128);
	}

	Git& GetMergeMessage(Upp::String& message) {
		message.Clear();
		Upp::String file = AppendFileName(GetGitDir(), ".git/MERGE_MSG");
		Upp::FileIn in(file);
		while (!in.IsEof()) {
			Upp::String line = in.GetLine();
			if (*line != '#')
				message.Cat(line + "\r\n");
		}
		return *this;
	}
	Upp::String GetMergeMessage() {
		Upp::String val;
		GetMergeMessage(val);
		return (val);
	}

	Git& GetUnmergedFiles(Upp::Vector<Upp::String>& filelist) {
		Upp::Vector<Upp::String> output = CmdExecute("diff --name-status --diff-filter=U").GetOutput();
		for (int i = 0; i < output.GetCount(); ++i) {
			if (output[i].GetCount() && output[i].Mid(0, 2) == "U ")
				filelist.Add(TrimBoth(output[i].Mid(1)));
		}
		return *this;
	}
	bool IsUnmergedFiles() {
		Upp::Vector<Upp::String> filelist;
		GetUnmergedFiles(filelist);
		return (filelist.GetCount());
	}

	Git& Stash(const Upp::String& message);
	Git& StashApply(const Upp::String& stashname) {
		CmdExecute("stash pop --index " << stashname);
		return *this;
	}
	Git& StashList() {
		CmdExecute("stash list");
		return *this;
	}
	bool IsStashList() {
		int error = StashList().GetCmdErrorCode();
		return (error == 0 && GetOutput().GetCount() > 0);
	}

	Git& Reset(const Upp::Vector<Upp::String>& filelist, bool head);

	Git& GetLog(bool firstparent);
	Git& GetFileLog(const Upp::String& filepath) {
		CmdExecute("log --pretty=format:\"commit %H >< %an >< %at\" --follow -M -p -- " << filepath);
		return *this;
	}
	Git& GetHashLog() {
		CmdExecute("log --pretty=format:\"%H >< %s\" --all --no-color");
		return *this;
	}

	Git& Blame(const Upp::String& filepath) {
		// GetBranchLocal() returning current branch is workaround for git problem
		// If a file contained CRLF line endings in a repository with core.autocrlf=input,
		// then blame always marked lines as "Not Committed Yet", even if they were unmodified.
		CmdExecute("blame " << GetBranchLocal() << " --line-porcelain -M -- " << filepath);
		return *this;
	}

	Git& TagAdd(const Upp::String& hash, const Upp::String& tagname, const Upp::String& message,
				bool force = false);
	Git& TagDelete(const Upp::String& tagname) {
		CmdExecute("tag -d " << tagname);
		return *this;
	}
	Git& TagGetDesc(const Upp::String& tagname) {
		CmdExecute("tag -n -l " << tagname);
		return *this;
	}

	char GetConfigChangeType(Upp::String& oldvalue, Upp::String& newvalue);
	Git& ProcessStashApply();
	Git& ProcessBranchAdd(const Upp::String& startpoint);

	void ProcessUpdateStatus(const Upp::Vector<Upp::String>& gitdirs);
	void ProcessUpdateStatus() {
		Upp::Vector<Upp::String> tmp;
		ProcessUpdateStatus(tmp);
	}

	virtual void ProcessCommandOutput();

	int EditCommitMessage(const Upp::String& message, const Upp::Vector<Upp::String>& parameters, Upp::String& msgpath);

	Git(){};
	Git(const Upp::String& path) : gitdir(path){};

	virtual ~Git(){};
};

#endif

// vim: ts=4
