#include "GitCommitData.h"

using namespace Upp;

GitCommitData::GitCommitData()
{
	CtrlLayout(*this);
	summary.NoEofLine();
}

void GitCommitData::ClearData()
{
	treeval.SetText("");
	parentval.SetText("");
	parentfile.SetText("");
	authorval.SetText("");
	authortime.SetText("");
	committerval.SetText("");
	committertime.SetText("");
	summary.SetData("");
}

void GitCommitData::ShowCommitData(const String &folder, const String& hash, const String& filename)
{
	ClearData();

	if (hash.GetCount() > 0) {
		SetGitDir(folder);

		String command = "log --pretty=format:\"%P >< %an >< %aE >< %at >< %cn >< %cE >< %ct\" " << hash;
		Vector<String> output = CmdExecute(command).GetOutput();

		if (!GetCmdErrorCode()) {
			Vector<String> tmp = Split(output[0], "><");
			if (tmp.GetCount() > 6) {
				treeval.SetText(hash.Left(GIT_HASH_SIZE));

				Vector<String> par = Split(tmp[0], " ");
				String parents;
				for (int i = 0; i < par.GetCount(); ++i) {
					if (i > 0)
						parents.Cat() << ", ";
					parents.Cat() << par[i].Left(GIT_HASH_SIZE);
				}
				parentval.SetText(parents);

				parentfile.SetText(filename);

				authorval.SetText(Format("%s  <%s>", TrimBoth(tmp[1]), TrimBoth(tmp[2])));
				authortime.SetText(Format(UPP::TimeFromUTC(ScanInt64(tmp[3]))));

				committerval.SetText(Format("%s  <%s>", TrimBoth(tmp[4]), TrimBoth(tmp[5])));
				committertime.SetText(Format(UPP::TimeFromUTC(ScanInt64(tmp[6]))));

				command = "show -s --pretty=format:\"%B\" " << hash;
				output = CmdExecute(command).GetOutput();

				String sum;
				for (int i = 0; i < output.GetCount(); ++i)
					sum.Cat() << output[i] << "\n";
				summary.SetData(sum);
			}
		}
	}
}

// vim: ts=4
