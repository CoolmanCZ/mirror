#ifndef _ugit_GitMerge_h_
#define _ugit_GitMerge_h_

#include <ugit/Git.h>
#include <ugit/GitDiffCtrl.h>

struct GitMerge : Upp::WithGitCommitLayout<Upp::ParentCtrl>, public Git {
  private:
	Upp::WithGitMergeBranchesLayout<Upp::Ctrl> mb;

	Upp::Splitter diff;
	Upp::ArrayCtrl cmd;
	GitDiffCtrl diffctrl;

	bool cmdshow;
	bool allowmerge;

	Upp::String actualbranch;

	Upp::Id GIT_FILE;

	void FilesChangeRow();
	void ShowMerge();
	void ShowDiff(const Upp::String& mergefrom, const Upp::String& mergeto);
	void ToggleCmd();

  public:
	typedef GitMerge CLASSNAME;

	Upp::Event<> WhenStatusChange;

	bool IsDiffOutput() { return diffctrl.GetCount(); }
	bool GetCmdStatus() { return cmdshow; };
	void SetCmdStatus(bool st) {
		cmdshow = st;
		ToggleCmd();
	};
	bool GetMergeStatus() { return allowmerge; };

	void Init(const Upp::String& gitdir);
	virtual bool Key(Upp::dword key, int count);
	void BranchChangeRow();
	virtual void ProcessCommandOutput();
	void ExecuteMerge(bool signoff);
	void SaveDiff();

	GitMerge();
	virtual ~GitMerge(){};
};

struct GitMerging : Upp::WithGitCommittingLayout<Upp::TopWindow> {
  private:
	int initdone;
	Upp::Array<GitMerge> merge;

	void MainClose();
	void Close();
	void Fetch();
	void Stash();
	void StashApply();
	void Merge();
	void SaveDiff();
	void ToggleSet();

  public:
	typedef GitMerging CLASSNAME;

	Upp::Event<> WhenStash;

	void Init(Upp::Vector<Upp::String>& gitdirs);

	GitMerging();
};

#endif

// vim: ts=4
