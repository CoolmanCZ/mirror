#include "GitConfigBranch.h"

using namespace Upp;

GitBranches::GitBranches()
{
	CtrlLayout(*this, "GIT branches configuration");

	this->WhenClose = THISBACK(Close);
	close.WhenPush = THISBACK(Close);

	tabs.WhenAction = THISBACK(CheckModify);

	Sizeable().Zoomable();
}

void GitBranches::CheckModify()
{
	int tab = tabs.Get();
	for (int i = 0; i < branches.GetCount(); ++i) {
		if (branches[i].IsModified()) {
			tabs.Set(i);
			if (PromptYesNo("Save modified data?"))
				branches[i].SaveChanges();
			else
				branches[i].CancelChanges();
			break;
		}
	}
	tabs.Set(tab);
}

void GitBranches::Init(Vector<String>& gitdirs)
{
	branches.Clear();
    tabs.Reset();

	for (int i = 0 ; i < gitdirs.GetCount(); ++i) {
		branches.Add().SetGitDir(gitdirs[i]);
		tabs.Add(branches[i].SizePos(), GetFileName(branches[i].GetGitDir()));
		branches[i].UpdateRemoteData();
		branches[i].ShowConfigBranch();
	}
}

GitConfigBranch::GitConfigBranch()
{
	CtrlLayout(*this);
	CtrlLayout(mirror, "Mirror remote branches to the local repository");

	ID1 = "ID1";
	ID2 = "ID2";
	ID3 = "ID3";
	ID3 = "ID4";

	branchdl <<= THISBACK(OnBranchDl);
	branchdl.WhenDrop = THISBACK(BranchDlWhenDrop);
	branchdl.NoDropFocus();
	branchdl.NoWantFocus();

	remotesdl <<= THISBACK(OnRemotesDl);
	remotesdl.NoDropFocus();
	remotesdl.NoWantFocus();

	remotemergedl <<= THISBACK(OnRemoteMergeDl);
	remotemergedl.NoDropFocus();
	remotemergedl.NoWantFocus();

	branchadd.WhenPush = THISBACK(BranchAdd);
	branchmod.WhenPush = THISBACK(BranchModify);
	branchdel.WhenPush = THISBACK(BranchDelete);
	branchcheckout.WhenPush   = THISBACK(BranchCheckout);
	branchpushremote.WhenPush = THISBACK(BranchPushRemote);
	branchdelremote.WhenPush  = THISBACK(BranchDeleteRemote);

	remoteadd.WhenPush = THISBACK(RemoteAdd);
	remotemod.WhenPush = THISBACK(RemoteModify);
	remotedel.WhenPush = THISBACK(RemoteDelete);
	remotemirror.WhenPush = THISBACK(RemoteMirror);

	mirror.Sizeable().Zoomable();
	mirror.Acceptor(mirror.ok, IDOK);
	mirror.ok.Ok().SetLabel("Mirror");
	mirror.Rejector(mirror.close, IDCANCEL);
	mirror.close.Cancel().SetLabel("Cancel");
	mirror.WhenClose = mirror.Rejector(IDCANCEL);

	mirror.cmd.AddColumn(ID1, "Remote branches");
	mirror.cmd.AddColumn(ID2, "Local branches").AddIndex(ID3).SetConvert(Single<ConvColor>());
	mirror.cmd.AddColumn(ID4,"").Ctrls<Option>().HeaderTab().Fixed(30);
	mirror.cmd.AddColumnAt(ID3, "").SetConvert(Single<ConvStatus>());
	mirror.cmd.NoWantFocus();
	mirror.cmd.WhenLeftDouble = THISBACK(RemoteMirrorEdit);
	mirror.cmd.WhenBar = THISBACK(RemoteMirrorBar);

	BackPaint();
}

void GitConfigBranch::BranchDlWhenDrop()
{
	if (IsModified() && PromptYesNo("Save modified data?"))
		SaveChanges();
}

void GitConfigBranch::OnBranchDl()
{
	String branch = ~branchdl;
	actualbranch = branch;
	remotesdl.Clear();

	if (IsGitDir(GetGitDir())) {
		branchadd.Enable();
		String remote = GetBranchRemote(branch);
		Vector<String> remotes = GetRemotes().GetOutput();

		if (!GetCmdErrorCode()) {
			remotesdl.Add("");
			for (int i = 0; i < remotes.GetCount(); ++i)
				remotesdl.Add(remotes[i]);
			if (remote.IsEmpty())
				remotesdl.GoBegin();
			else
				remotesdl = remote;
		}

		branchdesc.SetText(GetBranchDescription(branch));
	} else {
		branchadd.Disable();
	}

	remotesdl.ClearModify();

	bool enabledel = (branchdl.GetCount() > 1 && branch.Compare(GetBranchLocal()) != 0);
	branchmod.Enable(branchdl.GetCount());
	branchdel.Enable(enabledel);
	branchcheckout.Enable(enabledel);

	OnRemotesDl();
}

void GitConfigBranch::OnRemotesDl()
{
	String branch = ~branchdl;
	String remote = ~remotesdl;
	remotemergedl.Clear();

	if (remote.IsEmpty()) {
		remotemod.Disable();
		remotedel.Disable();
		remotemergedl.Disable();
		remoteurl.SetText("");
	} else {
		remotemod.Enable();
		remotedel.Enable();
		remotemergedl.Enable();

		Vector<String> remotebranches = GetBranchAll(false).GetOutput();
		if (!GetCmdErrorCode()) {
			for (int i = 0; i < remotebranches.GetCount(); ++i) {
				String merge = TrimLeft(remotebranches[i]);
				if (merge.Find("->") >= 0)
					continue;
				if (merge.Find(remote) == 0)
					remotemergedl.Add(GetFileName(merge));
			}

			String mergebranch = GetBranchMerge(actualbranch);
			if (mergebranch.IsEmpty())
				remotemergedl.GoBegin();
			else
				remotemergedl = GetFileName(mergebranch);

			remoteurl.SetText(GetRemoteUrl(remote));
		}
	}

	remotesdl.Enable(branchdl.GetCount());
	remoteadd.Enable(branchdl.GetCount());
	remotemirror.Enable(remotemergedl.GetCount());

	OnRemoteMergeDl();

	remotemergedl.ClearModify();
}

void GitConfigBranch::OnRemoteMergeDl()
{
	String branch = ~branchdl;
	String remote = ~remotesdl;

	branchpushremote.Enable(remote.GetCount() > 0 && remotemergedl.Find(branch) == -1);
	branchdelremote.Enable(remote.GetCount() > 0 && remotemergedl.GetCount() > 1);
}

void GitConfigBranch::RemoteChange()
{
	String branch = ~branchdl;
	String remote = ~remotesdl;
	String oldremote = GetBranchRemote(branch);

	if (oldremote.Compare(remote) != 0) {
		char type = GetConfigChangeType(oldremote, remote);
		String name = "branch." << branch << ".remote";
		SetGitConfig(true, type, name, remote).ProcessCommandOutput();
	}
	remotesdl.ClearModify();
}

void GitConfigBranch::MergeChange()
{
	String branch = ~branchdl;
	String merge = ~remotemergedl;
	String oldmerge = GetFileName(GetBranchMerge(branch));

	if (oldmerge.Compare(merge) != 0) {
		char type = GetConfigChangeType(oldmerge, merge);
		String name = "branch." << branch << ".merge";
		String value = "refs/heads/" << merge;
		SetGitConfig(true, type, name, value).ProcessCommandOutput();
	}

	remotemergedl.ClearModify();
}

void GitConfigBranch::BranchAdd()
{
	ProcessBranchAdd(GetBranchLocal());
	ProcessCommandOutput();
	ShowConfigBranch();
}

void GitConfigBranch::BranchModify()
{
	WithGitConfigParamLayout2<TopWindow> branchModify;
	CtrlLayout(branchModify, "Modify the branch");

	branchModify.Acceptor(branchModify.ok, IDOK);
	branchModify.ok.Ok().SetLabel("Modify");
	branchModify.Rejector(branchModify.cancel, IDCANCEL);
	branchModify.cancel.Cancel();
	branchModify.WhenClose = branchModify.Rejector(IDCANCEL);
	branchModify.param1text.SetText("Branch name:");
	branchModify.param2text.SetText("Description:");

	String oldname = ~branchdl;
	String olddesc = branchdesc.GetText();

	branchModify.param1.SetData(oldname);
	branchModify.param1.ClearModify();
	branchModify.param2.SetData(olddesc);
	branchModify.param2.ClearModify();

	int c = branchModify.RunAppModal();
	if (c == IDOK) {
		if (branchModify.param2.IsModified()) {
			String desc = ~branchModify.param2;
			char type = GetConfigChangeType(olddesc, desc);
			String name = "branch." << oldname << ".description";
			SetGitConfig(true, type, name, desc);
		}
		if (branchModify.param1.IsModified()) {
			String name = ~branchModify.param1;
			BranchRename(oldname, name);
		}
		ProcessCommandOutput();
	}
	ShowConfigBranch();
}

void GitConfigBranch::BranchDelete()
{
	String branch = ~branchdl;

	if (branch.Compare(GetBranchLocal()) == 0) {
		ErrorOK(Format("Cannot delete the branch '%s' which you are currently on.", branch));
		return;
	}

	if (PromptYesNo(Format("Delete the branch '%s'?", branch))) {
		if (remotemergedl.Find(branch) != -1)
			BranchDeleteRemote();
		Git::BranchDelete(branch);
		ProcessCommandOutput();
		actualbranch.Clear();
		ShowConfigBranch();
	}
}

void GitConfigBranch::BranchCheckout()
{
	String branch = ~branchdl;

	if (PromptYesNo(Format("Checkout the branch '%s'?", branch))) {
		Git::BranchCheckout(branch);
		ProcessCommandOutput();
		ShowConfigBranch();
	}
}

void GitConfigBranch::BranchPushRemote()
{
	String branch = ~branchdl;
	String remote = ~remotesdl;

	bool exists = remote.GetCount() > 0 && remotemergedl.Find(branch) == -1;

	if (exists && PromptYesNo(Format("Push the branch '%s' to the remote repository '%s'?", branch, remote))) {
		Git::BranchPushRemote(remote, branch);
		ProcessCommandOutput();
		OnBranchDl();
	}
}

void GitConfigBranch::BranchDeleteRemote()
{
	String branch = ~remotemergedl;
	String remote = ~remotesdl;

	bool exists = remote.GetCount() > 0 && remotemergedl.GetCount() > 1;

	if (exists && PromptYesNo(Format("Delete the branch '%s' at the remote repository '%s'?", branch, remote))) {
		Git::BranchDeleteRemote(remote, branch);
		ProcessCommandOutput();
		String name = "branch." << branch << ".merge";
		SetGitConfig(true, 'U', name, "").ProcessCommandOutput();
		name = "branch." << branch << ".remote";
		SetGitConfig(true, 'U', name, "").ProcessCommandOutput();
		OnBranchDl();
	}
}

void GitConfigBranch::RemoteAdd()
{
	WithGitConfigParamLayout2<TopWindow> remoteAdd;
	CtrlLayout(remoteAdd, "Add new remote");

	remoteAdd.Acceptor(remoteAdd.ok, IDOK);
	remoteAdd.ok.Ok();
	remoteAdd.Rejector(remoteAdd.cancel, IDCANCEL);
	remoteAdd.cancel.Cancel();
	remoteAdd.WhenClose = remoteAdd.Rejector(IDCANCEL);
	remoteAdd.param1text.SetText("Remote name:");
	remoteAdd.param2text.SetText("Remote URL");

	int c = remoteAdd.RunAppModal();
	if (c == IDOK) {
		if (remoteAdd.param1.IsModified()) {
			String remote = TrimBoth(remoteAdd.param1.GetData().ToString());
			String remoteurl = TrimBoth(remoteAdd.param2.GetData().ToString());

			RemoteCreate(remote, remoteurl);
			ProcessCommandOutput();
			Fetch(remote);
		}
	}
	ShowConfigBranch();
}

void GitConfigBranch::RemoteModify()
{
	WithGitConfigParamLayout2<TopWindow> remoteModify;
	CtrlLayout(remoteModify, "Modify the remote");

	remoteModify.Acceptor(remoteModify.ok, IDOK);
	remoteModify.ok.Ok().SetLabel("Modify");
	remoteModify.Rejector(remoteModify.cancel, IDCANCEL);
	remoteModify.cancel.Cancel();
	remoteModify.WhenClose = remoteModify.Rejector(IDCANCEL);

	String oldname = ~remotesdl;
	String oldurl = remoteurl.GetText();

	remoteModify.param1.SetData(oldname);
	remoteModify.param1.ClearModify();
	remoteModify.param2.SetData(oldurl);
	remoteModify.param2.ClearModify();

	int c = remoteModify.RunAppModal();
	if (c == IDOK) {
		String url = ~remoteModify.param2;
		if (url.IsEmpty()) {
			ErrorOK("Remote URL can't be empty! Modification was canceled.");
		} else {
			if (remoteModify.param2.IsModified()) {
				char type = GetConfigChangeType(oldurl, url);
				String name = "remote." << oldname << ".url";
				SetGitConfig(true, type, name, url);
			}
			if (remoteModify.param1.IsModified()) {
				String name = ~remoteModify.param1;
				RemoteRename(oldname, name);
			}
			ProcessCommandOutput();
		}
	}
	ShowConfigBranch();
}

void GitConfigBranch::RemoteDelete()
{
	String remote = ~remotesdl;

	if (PromptYesNo(Format("Delete the remote '%s'?", remote))) {
		Git::RemoteDelete(remote);
		ProcessCommandOutput();
	}
	ShowConfigBranch();
}

void GitConfigBranch::RemoteMirror()
{
	Vector<String> localbranches = GetBranchAll(true).GetOutput();
	int localerrorcode = GetCmdErrorCode();
	Vector<String> remotebranches = GetBranchAll(false).GetOutput();
	int remoteerrorcode = GetCmdErrorCode();

	if (!remoteerrorcode && !localerrorcode && remotebranches.GetCount() > 0) {
		mirror.cmd.Clear();
		for (int i = 0; i < remotebranches.GetCount(); ++i) {
			String remotebr = TrimLeft(remotebranches[i]);
			String localbr = GetFileName(remotebr);
			int status = 0;
			if (remotebr.Find("->") >= 0)
				continue;

			for (int j = 0; j < localbranches.GetCount(); ++j) {
				String local = localbranches[j][0] == '*' ? localbranches[j].Mid(2) : localbranches[j];
				String remote = GetBranchRemote(local) + "/" + GetFileName(GetBranchMerge(local));
				if (remotebr.Find(remote) == 0) {
					localbr = local;
					status = 1;
					break;
				}
			}
			if (mirror.cmd.Find(localbr, ID2) >= 0)
				status = 2;
			if (remotebr.Find("origin/") < 0)
				status = 3;
			mirror.cmd.Add(remotebr, TrimBoth(localbr), status);
		}

		RemoteMirrorCheckDuplicates();

		Vector<String> output;
		for (;;) {
			int c = mirror.RunAppModal();
			if (c == IDOK) {
				Vector<String> remotes = GetRemotes().GetOutput();

				if (!GetCmdErrorCode()) {
					for (int i = 0; i < mirror.cmd.GetCount(); ++i) {
						if (mirror.cmd.Get(i, ID4) == 1 && mirror.cmd.Get(i, ID3) == 2)
							ErrorOK("Duplicate name of the local branch!");

						if (mirror.cmd.Get(i, ID4) == 1 && mirror.cmd.Get(i, ID3) == 1)
							ErrorOK("The local branch has already existed!");

						if (mirror.cmd.GetCtrl(i, 2)->GetData() == 1) {
							for (int j = 0; j < remotes.GetCount(); ++j) {
								if (mirror.cmd.Get(i, ID1).ToString().Find(remotes[j]) == 0) {
									String remote = remotes[j];
									String upstream = mirror.cmd.Get(i, ID1);
									String remotebranch = upstream.Mid(remote.GetCharCount()+1);
									String localbranch = mirror.cmd.Get(i, ID2);
									output.Append(BranchFetchRemote(remote, remotebranch, localbranch).GetOutput());
									output.Append(BranchSetMerge(localbranch, upstream).GetOutput());
									break;
								}
							}
						}
					}
				}
				if (output.GetCount() > 0) {
					SetOutput(output);
					ProcessCommandOutput();
					ShowConfigBranch();
				}
				mirror.Close();
				break;
			}
			if (c == IDCANCEL) {
				mirror.Close();
				break;
			}
		}
	}
}

void GitConfigBranch::RemoteMirrorBar(Bar& bar)
{
	int row = mirror.cmd.GetCursor();
	if (row < 0)
		return;

	bool edit = (mirror.cmd.Get(row, ID3) != 1);

	bar.Add(edit, "Edit branch name", THISBACK(RemoteMirrorEdit));
}

void GitConfigBranch::RemoteMirrorEdit()
{
	int row = mirror.cmd.GetCursor();
	if (row >= 0 && mirror.cmd.Get(row, ID3) != 1) {
		WithGitConfigParamLayout1<TopWindow> brname;
		CtrlLayout(brname, "Change the branch name");

		brname.Acceptor(brname.ok, IDOK);
		brname.ok.Ok().SetLabel("Change");
		brname.Rejector(brname.cancel, IDCANCEL);
		brname.cancel.Cancel().SetLabel("Cancel");
		brname.WhenClose = brname.Rejector(IDCANCEL);

		brname.param1text.SetText("Branch name:");
		brname.param1 <<= mirror.cmd.Get(ID2);
		brname.param1.ClearModify();

		int c = brname.RunAppModal();
		if (c == IDOK && brname.param1.IsModified()) {
			mirror.cmd.Set(row, ID2, brname.param1.GetData());
		}
		brname.Close();
		RemoteMirrorCheckDuplicates();
	}
}

void GitConfigBranch::RemoteMirrorCheckDuplicates()
{
	for (int i = 0; i < mirror.cmd.GetCount(); ++i) {
		if (mirror.cmd.Get(i, ID3) == 3) {
			mirror.cmd.SetLineColor(i, LtGray());
			mirror.cmd.GetCtrl(i, 2)->Disable();
			mirror.cmd.DisableLine(i);
			continue;
		}
		String name = mirror.cmd.Get(i, ID2);
		int pos = mirror.cmd.Find(name, ID2);
		if (pos >= 0 && pos != i) {
			mirror.cmd.Set(i, ID3, 2);
		} else {
			if (mirror.cmd.Get(i, ID3) != 1)
				mirror.cmd.Set(i, ID3, 0);
		}
	}

	for (int i = 0; i < mirror.cmd.GetCount(); ++i) {
		if (mirror.cmd.Get(i, ID3) == 3) {
			mirror.cmd.SetLineColor(i, LtGray());
			mirror.cmd.GetCtrl(i, 2)->Disable();
			mirror.cmd.DisableLine(i);
			continue;
		}
		int status = mirror.cmd.Get(i, ID3);
		if (status > 0) {
			mirror.cmd.GetCtrl(i, 2)->Disable();
			mirror.cmd.Set(i, ID4, 0);
		} else {
			mirror.cmd.GetCtrl(i, 2)->Enable();
			mirror.cmd.Set(i, ID4, 1);
		}
	}
}

void GitConfigBranch::ProcessCommandOutput()
{
	Vector<String> output = GetOutput();
	if (output.GetCount() > 0) {
		WithGitCmdOutputLayout<TopWindow> outputLay;
		CtrlLayout(outputLay, "GIT command output");
                outputLay.ok.Hide();

		outputLay.Rejector(outputLay.close, IDCANCEL);
		outputLay.close.Cancel();
		outputLay.WhenClose = outputLay.Rejector(IDCANCEL);

		outputLay.cmd.AddColumn("Command output");
		outputLay.cmd.NoGrid().NoHeader();
		outputLay.cmd.NoWantFocus();

		outputLay.cmd.Clear();
		for (int i = 0; i < output.GetCount(); ++i)
			outputLay.cmd.Add(output[i]);

		outputLay.RunAppModal();
	}
}

void GitConfigBranch::UpdateRemoteData()
{
	if (IsGitDir(GetGitDir())) {
		Vector<String> remotes = GetRemotes().GetOutput();
		if (!GetCmdErrorCode()) {
			for (int i = 0; i < remotes.GetCount(); ++i) {
				String command = "remote update " << remotes[i] << " --prune";
				CmdExecute(command);
				if (GetCmdErrorCode())
					ProcessCommandOutput();
			}
		}
	}
}

void GitConfigBranch::ShowConfigBranch()
{
	branchdl.Clear();
	if (IsGitDir(GetGitDir())) {
		Vector<String> branches = GetBranchAll().GetOutput();
		if (!GetCmdErrorCode()) {
			for (int i = 0; i < branches.GetCount(); ++i) {
				String branch = TrimBoth(branches[i]);
				if (branch.Find("*") == 0) {
					branchdl.Add(branch.Mid(2), AttrText(branch.Mid(2)).Bold());
					actualbranch = branch.Mid(2);
				} else {
					branchdl.Add(branch);
				}
			}
		}
		branchdl = actualbranch;
	}
	OnBranchDl();
}

// vim: ts=4
