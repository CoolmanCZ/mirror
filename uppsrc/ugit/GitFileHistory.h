#ifndef _ugit_GitFileHistory_h_
#define _ugit_GitFileHistory_h_

#include <ugit/Git.h>
#include <ugit/GitCommitData.h>
#include <ugit/GitDiffCtrl.h>

struct GitFileHistory : Upp::WithGitSplitterLayout<Upp::TopWindow>, public Git {
  private:
	struct ConvHash : Upp::Convert {
		Upp::Value Format(const Upp::Value& q) const {
			return q.IsNull() ? "" : q.ToString().Left(GIT_HASH_SIZE);
		}
	};

	struct ConvDateTime : Upp::Convert {
		Upp::Value Format(const Upp::Value& q) const {
			return q.IsNull() ? "" : Upp::Format(Upp::TimeFromUTC(q)).ToString().Left(10);
		}
	};

	Upp::Splitter commitoutput;
	Upp::ArrayCtrl commitlist;
	GitDiffCtrl diffctrl;
	GitCommitData commitdata;

	Upp::Id GIT_HASH;
	Upp::Id GIT_AUTHOR;
	Upp::Id GIT_AUTHOR_TIME;
	Upp::Id GIT_DIFF;

	void ShowDetails(void);

  public:
	typedef GitFileHistory CLASSNAME;

	virtual bool Key(Upp::dword key, int count);
	void ShowFileHistory(const Upp::String& filepath);

	GitFileHistory();
	virtual ~GitFileHistory(){};
};

#endif

// vim: ts=4
