#ifndef _ugit_GitCommitData_h_
#define _ugit_GitCommitData_h_

#include <ugit/Git.h>

struct GitCommitData : Upp::WithGitCommitDataLayout<Upp::Ctrl>, public Git {
  public:
	typedef GitCommitData CLASSNAME;

	void ClearData();
	void ShowCommitData(const Upp::String& folder, const Upp::String& hash, const Upp::String& filename);
	void ShowCommitData(const Upp::String& folder, const Upp::String& hash) { return ShowCommitData(folder, hash, ""); }

	GitCommitData();
	virtual ~GitCommitData(){};
};

#endif

// vim: ts=4
