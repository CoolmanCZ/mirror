#ifndef _ugit_GitHistory_h_
#define _ugit_GitHistory_h_

#include <ugit/Git.h>
#include <ugit/GitCommitData.h>
#include <ugit/GitDiffCtrl.h>

struct GitHistory : Upp::WithGitHistoryLayout<Upp::ParentCtrl>, public Git {
  private:
	struct ConvHash : Upp::Convert {
		Upp::Value Format(const Upp::Value& q) const {
			return q.IsNull() ? "" : q.ToString().Left(GIT_HASH_SIZE);
		}
	};

	enum type {
		typediff  = 1,
		typepatch = 2
	};

	Upp::ArrayCtrl log;
	Upp::Splitter diff;
	Upp::ArrayCtrl files;
	GitDiffCtrl diffctrl;
	GitCommitData commitdata;

	int diffrow, diffpos, commitpos;

	Upp::Id GIT_TREE;
	Upp::Id GIT_HASH;
	Upp::Id GIT_REFS;
	Upp::Id GIT_SUBJECT;
	Upp::Id GIT_STATUS;
	Upp::Id GIT_FILE;

	void SwitchChange();
	void StatusMenuBar(Upp::Bar& bar);
	void BranchAdd();
	void TagInsert();
	void TagAction(bool show);
	bool CheckError(const Upp::Vector<Upp::String>& test);
	void SaveDiff();
	void SavePatch();
	void SaveOutput(const Upp::String& hash, int type);
	void LogChangeRow();
	void FilesChangeRow();
	void SetCommitData(int row);

  public:
	typedef GitHistory CLASSNAME;

	virtual bool Key(Upp::dword key, int count);
	void ShowHistory();

	GitHistory();
};

struct GitHistories : Upp::WithGitCommittingLayout<Upp::TopWindow> {
  private:
	Upp::Array<GitHistory> history;

	void SetPanel();

  public:
	typedef GitHistories CLASSNAME;

	void Init(Upp::Vector<Upp::String>& gitdirs);

	GitHistories();
};

#endif

// vim: ts=4
