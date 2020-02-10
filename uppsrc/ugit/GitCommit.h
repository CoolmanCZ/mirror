#ifndef _ugit_GitCommit_h_
#define _ugit_GitCommit_h_

#include <ugit/Git.h>
#include <ugit/GitDiffCtrl.h>

struct GitCommit : Upp::WithGitCommitLayout<Upp::ParentCtrl>, public Git {
  private:
	struct ConvStatus : Upp::Convert {
		Upp::Value Format(const Upp::Value& q) const {
			Upp::AttrText result;
			Upp::String res = q.ToString();
			switch (res[0]) {
			case ' ':
				result.Set("unmodified");
				if (res[1] == 'M')
					result.Set("updated");
				if (res[1] == 'D')
					result.Set("deleted in work tree");
				break;
			case 'M':
				result.Set("updated");
				if ((res[1] == 'M') || (res[1] == 'D'))
					result.Set("updated in index");
				break;
			case 'A':
				result.Set("added");
				if (res[1] == 'A')
					result.Set("unmerged, both added").Paper(Upp::LtRed());
				if ((res[1] == 'M') || (res[1] == 'D'))
					result.Set("added to index");
				if (res[1] == 'U')
					result.Set("unmerged, added by us").Paper(Upp::LtRed());
				break;
			case 'D':
				result.Set("deleted");
				if (res[1] == 'M')
					result.Set("deleted from index");
				if (res[1] == 'D')
					result.Set("unmerged, both deleted").Paper(Upp::LtRed());
				if (res[1] == 'U')
					result.Set("unmerged, deleted by us").Paper(Upp::LtRed());
				break;
			case 'R':
				result.Set("renamed");
				if ((res[1] == 'M') || (res[1] == 'D'))
					result.Set("renamed in index");
				break;
			case 'C':
				result.Set("copied");
				if ((res[1] == 'M') || (res[1] == 'D'))
					result.Set("copied in index");
				break;
			case 'U':
				result.Set("updated but unmerged");
				if (res[1] == 'D')
					result.Set("unmerged, deleted by them").Paper(Upp::LtRed());
				if (res[1] == 'A')
					result.Set("unmerged, added by them").Paper(Upp::LtRed());
				if (res[1] == 'U')
					result.Set("unmerged, both modified").Paper(Upp::LtRed());
				break;
			case '?':
				result.Set("untracked");
				break;
			case '!':
				result.Set("ignored");
				break;
			default:
				result.Set("uknown");
				break;
			}
			return (result);
		}
	};

	Upp::WithGitCommitFilesLayout<Upp::Ctrl> cf;

	Upp::Splitter diff;
	Upp::ArrayCtrl cmd;
	GitDiffCtrl diffctrl;

	bool cmdshow;
	bool allowcommit;

	Upp::Id GIT_SELECT;
	Upp::Id GIT_STATUS;
	Upp::Id GIT_FILE;

	void StatusChangeRow();
	void StatusMenuBar(Upp::Bar& bar);
	void CancelCheckout();
	void CancelDelete();
	void Unstage();
	void Reset();
	void AddToRepository();
	void AddToGitignore();
	void DelFromRepository();
	void DelFile();
	void GetFilePos(const Upp::String& filename, int filepos);
	void ToggleDiffType();
	void ToggleCmd();
	void GetStatusList();

  public:
	typedef GitCommit CLASSNAME;

	Upp::Event<> WhenStatusChange;
	Upp::Event<> WhenRepositoryChange;
	Upp::Event<Upp::String, int> WhenLeftDouble;
	void DoRepositoryChange() { WhenRepositoryChange(); }

	bool GetCmdStatus() { return cmdshow; };
	void SetCmdStatus(bool st) {
		cmdshow = st;
		ToggleCmd();
	};
	bool GetCommitStatus() { return allowcommit; };

	void Init(const Upp::String& gitdir);
	virtual bool Key(Upp::dword key, int count);
	virtual void ProcessCommandOutput();
	void ShowCommit();
	void ExecuteCommit(bool signoff);
	void SaveDiff();

	GitCommit();
	virtual ~GitCommit(){};
};

struct GitCommitting : Upp::WithGitCommittingLayout<Upp::TopWindow> {
  private:
	int initdone;
	Upp::Array<GitCommit> commit;

	void MainClose();
	void Close();
	void GetFilePos(const Upp::String& filepath, int filepos);
	void Stash();
	void StashApply();
	void Commit();
	void SaveDiff();
	void ToggleSet();

  public:
	typedef GitCommitting CLASSNAME;

	Upp::Event<> WhenStash;
	Upp::Event<> WhenRepositoryChange;
	Upp::Event<Upp::String, int> WhenLeftDouble;
	void DoRepositoryChange() { WhenRepositoryChange(); }

	void Init(Upp::Vector<Upp::String>& gitdirs, const Upp::String& packagepath);

	GitCommitting();
};

#endif

// vim: ts=4
