#ifndef _ugit_GitConfigBranch_h_
#define _ugit_GitConfigBranch_h_

#include <ugit/Git.h>

struct GitConfigBranch : Upp::WithGitConfigBranchLayout<Upp::ParentCtrl>, public Git {
  private:
	struct ConvColor : Upp::Convert {
		Upp::Value Format(const Upp::Value& q) const {
			Upp::ValueArray va = q;
			Upp::AttrText result(va[0]);
			if (va[1] == 1)
				result.Paper(Upp::LtGreen());
			if (va[1] == 2)
				result.Paper(Upp::LtRed());
			return (result);
		}
	};

	struct ConvStatus : Upp::Convert {
		Upp::Value Format(const Upp::Value& q) const {
			if (q == 1)
				return ("Already mirrored");
			else if (q == 2)
				return ("Duplicate name");
			else if (q == 3)
				return ("It is not the origin");
			else
				return ("");
		}
	};

	Upp::Id ID1;
	Upp::Id ID2;
	Upp::Id ID3;
	Upp::Id ID4;

	Upp::WithGitCmdOutputLayout<Upp::TopWindow> mirror;

	Upp::String actualbranch;

	void BranchDlWhenDrop();
	void OnBranchDl();
	void OnRemotesDl();
	void OnRemoteMergeDl();
	void RemoteChange();
	void MergeChange();
	void BranchAdd();
	void BranchModify();
	void BranchDelete();
	void BranchCheckout();
	void BranchPushRemote();
	void BranchDeleteRemote();
	void RemoteAdd();
	void RemoteModify();
	void RemoteDelete();
	void RemoteMirror();
	void RemoteMirrorBar(Upp::Bar& bar);
	void RemoteMirrorEdit();
	void RemoteMirrorCheckDuplicates();

  public:
	typedef GitConfigBranch CLASSNAME;

	virtual void ProcessCommandOutput();

	virtual bool IsModified() const { return (remotesdl.IsModified() | remotemergedl.IsModified()); };
	void SaveChanges() {
		RemoteChange();
		MergeChange();
	};
	void CancelChanges() { OnBranchDl(); };

	void UpdateRemoteData();
	void ShowConfigBranch();

	GitConfigBranch();
	virtual ~GitConfigBranch(){};
};

struct GitBranches : Upp::WithGitTabsLayout<Upp::TopWindow> {
  private:
	Upp::Array<GitConfigBranch> branches;

	void CheckModify();
	void Close() {
		CheckModify();
		Break();
	};

  public:
	typedef GitBranches CLASSNAME;

	void Init(Upp::Vector<Upp::String>& gitdirs);

	GitBranches();
};

#endif

// vim: ts=4
