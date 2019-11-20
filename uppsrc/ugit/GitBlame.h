#ifndef _ugit_GitBlame_h_
#define _ugit_GitBlame_h_

#include <CodeEditor/CodeEditor.h>

#include <ugit/Git.h>
#include <ugit/GitCommitData.h>

struct GitBlame : Upp::WithGitSplitterLayout<Upp::TopWindow>, public Git {
  private:
	struct ConvDateTime : Upp::Convert {
		Upp::Value Format(const Upp::Value& q) const {
			return q.IsNull() ? "" : Upp::Format(Upp::TimeFromUTC(q)).ToString().Left(10);
		}
	};

	struct ConvHash : Upp::Convert {
		Upp::Value Format(const Upp::Value& q) const {
			return q.IsNull() ? "" : q.ToString().Left(GIT_HASH_SIZE);
		}
	};

	Upp::Splitter split;
	Upp::CodeEditor lines;
	Upp::ArrayCtrl blame;
	GitCommitData commitdata;
	int dosel = 0;

	Upp::Id GIT_HASH;
	Upp::Id GIT_HASH_FILE;
	Upp::Id GIT_AUTHOR;
	Upp::Id GIT_AUTHOR_TIME;
	Upp::Id GIT_FILENAME;

	void StatusMenuBar(Upp::Bar& bar);
	void ViewColumn(const Upp::Id& id);
	void ShowDetails();
	void SetLine();

  public:
	typedef GitBlame CLASSNAME;

	virtual bool Key(Upp::dword key, int count);
	void ShowBlame(const Upp::String& filepath);

	GitBlame();
	virtual ~GitBlame(){};
};

#endif

// vim: ts=4
