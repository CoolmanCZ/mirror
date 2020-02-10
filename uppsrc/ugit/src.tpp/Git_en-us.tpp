topic "Git command wrapper";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0; [*@7;4 Git]]}}&]
[s3; &]
[s1;:Git`:`:struct: [@(0.0.255)3 struct][3 _][*3 Git]&]
[s2;%% Envelopes most of the basic commands used for working with 
the GIT repository. Commands are executed in the GIT directory 
which is set by [* SetGitDir()] command.&]
[s2;%% Command output and execution error code are available for 
last command only.&]
[s0;%% &]
[s3; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s4; &]
[s5;:Git`:`:SetGitDir`(const Upp`:`:String`&`): [@(0.0.255) void]_[* SetGitDir]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path])&]
[s2; Set GIT working directory for executing GIT commands.&]
[s7; [*@3 path]-|GIT directory path&]
[s3; &]
[s4; &]
[s5;:Git`:`:GetGitDir`(`): [_^Upp`:`:String^ String]_[* GetGitDir]()&]
[s2; Return GIT working directory.&]
[s7; [*/ Return value]-|GIT directory path&]
[s0;0 &]
[s4; &]
[s5;:Git`:`:SetOutput`(Upp`:`:Vector`<Upp`:`:String`>`&`): [_^Git^ Git][@(0.0.255) `&]_[* S
etOutput]([_^Upp`:`:Vector^ Vector]<[_^Upp`:`:String^ String]>`&_[*@3 out])&]
[s2; Function for rewrite GIT command output.&]
[s7; [*@3 out]-|Data for output rewriting&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetOutput`(`): [_^Upp`:`:Vector^ Vector]<[_^Upp`:`:String^ String]>_[* GetOutpu
t]()&]
[s2;%% Return GIT command output.&]
[s3; &]
[s4; &]
[s5;:Git`:`:IsOutput`(`): [@(0.0.255) bool]_[* IsOutput]()&]
[s2;%% Return [*@3 true] if the last GIT command output contains data 
otherwise return [*@3 false].&]
[s3; &]
[s4; &]
[s5;:Git`:`:GetCmdErrorCode`(`): [@(0.0.255) int]_[* GetCmdErrorCode]()&]
[s2;%% Return error code of the last GIT command.&]
[s3; &]
[s4; &]
[s5;:Git`:`:CmdExecute`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* CmdExecute](
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 cmd])&]
[s2; Execute GIT command in the GIT working directory.&]
[s7; [*@3 cmd]-|Git command to execute&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:CmdExecute`(const Upp`:`:String`&`,const Upp`:`:Vector`<Upp`:`:String`>`&`): [_^Git^ G
it][@(0.0.255) `&]_[* CmdExecute]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 cmd], [@(0.0.255) const]_[_^Upp`:`:Vector^ Vector]<[_^Upp`:`:String^ String]>`&_[*@3 p
arameters])&]
[s2; Execute GIT command in the GIT working directory with list of 
command parameters.&]
[s7; [*@3 cmd]-|Git command to execute&]
[s7;%% [%-*@3 parameters]-|List of command parameters&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:InitRepository`(`): [_^Git^ Git][@(0.0.255) `&]_[* InitRepository]()&]
[s2;%% Initialize actual package directory as a Git repository. The 
new Git repository is empty by default, so you have to add files 
to this repository manually.&]
[s3; &]
[s4; &]
[s5;:Git`:`:GetStatus`(`): [_^Git^ Git][@(0.0.255) `&]_[* GetStatus]()&]
[s2; Give the output in an easy`-to`-parse format for scripts. This 
is similar to the short output, but will remain stable across 
Git versions and regardless of user configuration. See below 
for details.&]
[s3; &]
[s4; &]
[s5;:Git`:`:GetStatusVerbose`(`): [_^Git^ Git][@(0.0.255) `&]_[* GetStatusVerbose]()&]
[s2; Show the working tree status.&]
[s3; &]
[s4; &]
[s5;:Git`:`:GetStatusTracked`(`): [_^Git^ Git][@(0.0.255) `&]_[* GetStatusTracked]()&]
[s2; Similar to the [* GitCmdGetStatus ]but only tracked files are 
listed.&]
[s3; &]
[s4; &]
[s5;:Git`:`:Fetch`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* Fetch]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 parameters])&]
[s2; Fetches named heads from a remote repository, along with the 
objects necessary to complete them. After fetching, remove any 
remote`-tracking references that no longer exist on the remote.&]
[s7; [*@3 parameters]-|Remote repository name&]
[s3; &]
[s4; &]
[s5;:Git`:`:FetchAll`(`): [_^Git^ Git][@(0.0.255) `&]_[* FetchAll]()&]
[s2; [%% Call the ][* GitCmdFetch] to fetch named heads from all remote 
repositories in the GIT directory.&]
[s3; &]
[s4; &]
[s5;:Git`:`:Push`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* Push]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 parameters])&]
[s2; Update remote refs, using local refs, while sending objects 
necessary to complete the given refs. Remove remote branches 
that don’t have a local counterpart.&]
[s7; [*@3 parameters]-|Remote repository name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:PushAll`(`): [_^Git^ Git][@(0.0.255) `&]_[* PushAll]()&]
[s2;%% Call the [%-* GitCmdPush][%-  ]to update all remote refs in the 
GIT directory.&]
[s3; &]
[s4; &]
[s5;:Git`:`:Commit`(const Upp`:`:Vector`<Upp`:`:String`>`&`,const Upp`:`:String`&`): [_^Git^ G
it][@(0.0.255) `&]_[* Commit]([@(0.0.255) const]_[_^Upp`:`:Vector^ Vector]<[_^Upp`:`:String^ S
tring]>`&_[*@3 parameters], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 m
sgpath])&]
[s2; Store the current contents of the index in a new commit along 
with a log message from the user describing the changes.&]
[s7;%% [%-*@3 parameters][%- -|]List of command parameters&]
[s7;%% [%-*@3 msgpath]-|File containing the commit message&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:IsCommitList`(`): [@(0.0.255) bool]_[* IsCommitList]()&]
[s2;%% Determine if the files for commit exist in the GIT directory.&]
[s7; [*/ Return value]-|[*@3 true] `- there are files for commit&]
[s3; &]
[s4; &]
[s5;:Git`:`:Unstage`(const Upp`:`:Vector`<Upp`:`:String`>`&`): [_^Git^ Git][@(0.0.255) `&
]_[* Unstage]([@(0.0.255) const]_[_^Upp`:`:Vector^ Vector]<[_^Upp`:`:String^ String]>`&_[*@3 f
ilelist])&]
[s2;%% Unstage and remove paths only from the index. Working tree 
files, whether modified or not, will be left alone.&]
[s7;%% [%-*@3 filelist][%- -|]List of the files to unstage from index&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:CancelCheckout`(const Upp`:`:Vector`<Upp`:`:String`>`&`): [_^Git^ Git][@(0.0.255) `&
]_[* CancelCheckout]([@(0.0.255) const]_[_^Upp`:`:Vector^ Vector]<[_^Upp`:`:String^ Strin
g]>`&_[*@3 filelist])&]
[s2; Check out a list of the files which are out of the index.&]
[s7;%% [%-*@3 filelist]-|List of the files to checkout cancel&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:AddToRepository`(const Upp`:`:Vector`<Upp`:`:String`>`&`): [_^Git^ Git][@(0.0.255) `&
]_[* AddToRepository]([@(0.0.255) const]_[_^Upp`:`:Vector^ Vector]<[_^Upp`:`:String^ Stri
ng]>`&_[*@3 filelist])&]
[s2; This command updates the index using the current content found 
in the working tree, to prepare the content staged for the next 
commit. It only adds the content of the specified file list.&]
[s7;%% [%-*@3 filelist]-|List of the files to add to the repository&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:AddToRepository`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* AddToR
epository]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 filepath])&]
[s2; This command updates the index using the current content found 
in the working tree, to prepare the content staged for the next 
commit. It only adds the content of the specified file.&]
[s7;%% [%-*@3 filepath][%- -|The f]ile name to add to the repository&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:AddToGitignore`(const Upp`:`:Vector`<Upp`:`:String`>`&`): [_^Git^ Git][@(0.0.255) `&
]_[* AddToGitignore]([@(0.0.255) const]_[_^Upp`:`:Vector^ Vector]<[_^Upp`:`:String^ Strin
g]>`&_[*@3 filelist])&]
[s2; This command appends list of the files to the [/ .gitignore] file 
in the local repository.&]
[s7;%% [%-*@3 filelist][%- -|List of the f]iles to add to the .gitignore 
file&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:AddToGitignore`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* AddToGi
tignore]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 filepath])&]
[s2; This command appends a file to the [/ .gitignore] file in the 
local repository.&]
[s7;%% [%-*@3 filepath][%- -|The f]ile name to add to the .gitignore file&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:DelFromRepository`(const Upp`:`:Vector`<Upp`:`:String`>`&`): [_^Git^ Git][@(0.0.255) `&
]_[* DelFromRepository]([@(0.0.255) const]_[_^Upp`:`:Vector^ Vector]<[_^Upp`:`:String^ St
ring]>`&_[*@3 filelist])&]
[s2; This command remove files from the index or from the working 
tree and the index.&]
[s7;%% [%-*@3 filelist]-|List of the files to remove from the repository&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:DelFromRepository`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* DelF
romRepository]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 filepath])
&]
[s2; This command remove the file from the index or from the working 
tree and the index.&]
[s7;%% [%-*@3 filepath]-|File name to remove from the repository&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:RenameInRepository`(const Upp`:`:String`&`,const Upp`:`:String`&`): [_^Git^ G
it][@(0.0.255) `&]_[* RenameInRepository]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 oldname], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 newname])
&]
[s2; It renames the file with name [*@3 oldname], which must exist 
and be either a file, symlink or directory, to the new name [*@3 newname].&]
[s7;%% [%-*@3 oldname]-|Old name&]
[s7;%% [%-*@3 newname]-|New name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:BranchCreate`(const Upp`:`:String`&`,const Upp`:`:String`&`,bool`): [_^Git^ G
it][@(0.0.255) `&]_[* BranchCreate]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 branchname], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 startp
oint], [@(0.0.255) bool]_[*@3 checkout])&]
[s2; The command creates a new branch head which points to the [*@3 startpoint]. 
If parameter checkout is set, this new branch is set as the current 
branch.&]
[s7;%% [%-*@3 branchname]-|Branch name&]
[s7; [%%*@3 startpoint][%% -|]The new branch head will point to this commit. 
It may be given as a branch name, a commit`-id, or a tag. If 
this option is omitted, the current HEAD will be used instead.&]
[s7;%% [%-*@3 checkout]-|[*@3 true] `- checkout the new created branch&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:BranchDelete`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* BranchDel
ete]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 branchname])&]
[s2; The command delete the branch [*@3 branchname]. Deleted branch 
must be fully merged in its upstream branch, or in HEAD if no 
upstream was set.&]
[s7;%% [%-*@3 branchname]-|Branch name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:BranchCheckout`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* BranchC
heckout]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 branchname])&]
[s2; Set branch [*@3 branchname] as the current branch.&]
[s7;%% [%-*@3 branchname][@3 -|]Branch name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:BranchRename`(const Upp`:`:String`&`,const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&
]_[* BranchRename]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 current
name], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 newname])&]
[s2; Rename a branch.&]
[s7;%% [%-*@3 currentname]-|Current branch name&]
[s7;%% [%-*@3 newname]-|New branch name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:BranchSetMerge`(const Upp`:`:String`&`,const Upp`:`:String`&`): [_^Git^ Git
][@(0.0.255) `&]_[* BranchSetMerge]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 localbranch], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 upstr
eam])&]
[s2; Set up [*@3 localbranch]`'s tracking information. [*@3 Upstream] 
is considered as a remote branch (e.g. remotes/origin/master).&]
[s7; [*@3 localbranch]-|Local branch name&]
[s7; [*@3 upstream]-|Remote branch name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:BranchFetchRemote`(const Upp`:`:String`&`,const Upp`:`:String`&`,const Upp`:`:String`&`): [_^Git^ G
it][@(0.0.255) `&]_[* BranchFetchRemote]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 remote], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 remotebran
ch], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 localbranch])&]
[s2; Fetch the branch [*@3 remotebranch] from the [*@3 remote] repository 
as [*@3 localbranch] to the local repository.&]
[s7; [*@3 remote]-|Remote repository&]
[s7; [*@3 remotebranch]-|Remote branch name&]
[s7; [*@3 localbranch]-|Local branch name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:BranchPushRemote`(const Upp`:`:String`&`,const Upp`:`:String`&`): [_^Git^ G
it][@(0.0.255) `&]_[* BranchPushRemote]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 remote], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 branchname
])&]
[s2; Push a local branch [*@3 branchname] to a [*@3 remote] Git repository 
and track it.&]
[s7; [*@3 remote]-|Remote repository&]
[s7; [*@3 branchname]-|Branch name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:BranchDeleteRemote`(const Upp`:`:String`&`,const Upp`:`:String`&`): [_^Git^ G
it][@(0.0.255) `&]_[* BranchDeleteRemote]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 remote], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 branchname
])&]
[s2; Delete remote branch [*@3 branchname] at the [*@3 remote] Git repository.&]
[s7; [*@3 remote]-|Remote repository&]
[s7; [*@3 branchname]-|Branch name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetBranchAll`(bool`): [_^Git^ Git][@(0.0.255) `&]_[* GetBranchAll]([@(0.0.255) bo
ol]_[*@3 local]_`=_[@(0.0.255) true])&]
[s2; List existing branches (local or remote).&]
[s7; [*@3 output][%% -|]Source to store command output&]
[s7;%% [%-*@3 local]-|[*@3 true] `- list local branches&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetBranchStatus`(const Upp`:`:String`&`,const Upp`:`:String`&`): [_^Upp`:`:String^ S
tring]_[* GetBranchStatus]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 l
ocalbranch], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 remotebranc
h])&]
[s2; Returns difference status between local and remote branches.&]
[s7; [*@3 localbranch]-|Local branch name&]
[s7; [*@3 remotebranch]-|Remote branch name&]
[s7; [*/ Return value]-|Commit status between branches&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetBranchStatusAll`(`): [_^Git^ Git][@(0.0.255) `&]_[* GetBranchStatusAll]()&]
[s2; Returns difference status between local and remote branches 
for all local branches. The output is one local branch status 
per line.&]
[s3; &]
[s4; &]
[s5;:Git`:`:GetBranchLocal`(Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* GetBranchLoca
l]([_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 branchname])&]
[s2; Returns the actual local branch.&]
[s7;%% [%-*@3 branchname]-|Source string to store branch name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetBranchLocalRemote`(Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* GetBran
chLocalRemote]([_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 remote])&]
[s2; Returns the remote branch pointed to the actual local branch.&]
[s7;%% [%-*@3 remote][%- -|]Source string to store branch name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetBranchRemote`(const Upp`:`:String`&`,Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&
]_[* GetBranchRemote]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 bran
chname], [_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 remote])&]
[s2; Returns the remote repository name pointed to the branch [*@3 branchname].&]
[s7; [*@3 branchname]-|Branch name&]
[s7;%% [%-*@3 remote]-|Source string to store remote branch name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetBranchMerge`(const Upp`:`:String`&`,Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&
]_[* GetBranchMerge]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 branc
hname], [_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 mergebranch])&]
[s2; Returns the upstream branch for the given branch [*@3 branchname]. 
Merge branch tells git fetch/git pull/git rebase which branch 
to merge and can also affect git push.&]
[s2; The merge information is used by git pull (which at first calls 
git fetch) to lookup the default branch for merging.&]
[s7;%% [%-*@3 branchname]-|Branch name&]
[s7;%% [%-*@3 mergebranch]-|Source string to store upstream branch&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetBranchDescription`(const Upp`:`:String`&`,Upp`:`:String`&`): [_^Git^ Git
][@(0.0.255) `&]_[* GetBranchDescription]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 branchname], [_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 description])&]
[s2; Returns description of the given branch [*@3 branchname].&]
[s7;%% [%-*@3 branchname]-|Branch name&]
[s7;%% [%-*@3 description][*@3  ]-|Source string to store the description&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetBranchLocal`(`): [_^Upp`:`:String^ String]_[* GetBranchLocal]()&]
[s2; Returns the actual local branch.&]
[s7; [*/ Returns value] -|Actual local branch&]
[s3; &]
[s4; &]
[s5;:Git`:`:GetBranchLocalRemote`(`): [_^Upp`:`:String^ String]_[* GetBranchLocalRemote](
)&]
[s2; Returns the remote branch pointed to the actual local branch.&]
[s7; [*/ Return value] -|The remote branch pointed to the actual local 
branch&]
[s3; &]
[s4; &]
[s5;:Git`:`:GetBranchRemote`(const Upp`:`:String`&`): [_^Upp`:`:String^ String]_[* GetBra
nchRemote]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 branchname])&]
[s2; Returns the remote repository name pointed to the branch [*@3 branchname].&]
[s7; [*@3 branchname]-|Branch name&]
[s7; [*/ Return value]-|The remote branch name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetBranchMerge`(const Upp`:`:String`&`): [_^Upp`:`:String^ String]_[* GetBran
chMerge]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 branchname])&]
[s2; Returns the upstream branch for the given branch [*@3 branchname]. 
Merge branch tells git fetch/git pull/git rebase which branch 
to merge and can also affect git push.&]
[s2; The merge information is used by git pull (which at first calls 
git fetch) to lookup the default branch for merging.&]
[s7;%% [%-*@3 branchname]-|Branch name&]
[s7; [*/ Return value][%% -|T]he upstream branch&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetBranchDescription`(const Upp`:`:String`&`): [_^Upp`:`:String^ String]_[* G
etBranchDescription]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 bra
nchname])&]
[s2; Returns the description of the given branch [*@3 branchname].&]
[s7;%% [%-*@3 branchname]-|Branch name&]
[s7; [*/ Return value][%% -|T]he description of the given branch&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:RemoteCreate`(const Upp`:`:String`&`,const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&
]_[* RemoteCreate]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 remoten
ame], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 remoteurl])&]
[s2;%% [%- Manage the set of repositories (`"remotes`") whose branches 
you track. A]dds a remote named [*@3 remotename] for the repository 
at [*@3 remoteurl] in the local configuration.&]
[s7;%% [%-*@3 remotename]-|Remote name&]
[s7;%% [%-*@3 remoteurl]-|Remote URL&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:RemoteDelete`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* RemoteDel
ete]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 remotename])&]
[s2; Manage the set of repositories (`"remotes`") whose branches 
you track. Remove remote repository named remotename from the 
local configuration.&]
[s7;%% [%-*@3 remotename]-|Remote name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:RemoteRename`(const Upp`:`:String`&`,const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&
]_[* RemoteRename]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 current
name], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 newname])&]
[s7; Rename the remote repository.&]
[s7;%% [%-*@3 currentname]-|Current remote name&]
[s7;%% [%-*@3 newname]-|New remote name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetRemotes`(`): [_^Git^ Git][@(0.0.255) `&]_[* GetRemotes]()&]
[s2; Returns a list of existing remotes.&]
[s3; &]
[s4; &]
[s5;:Git`:`:GetRemoteUrl`(const Upp`:`:String`&`,Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&
]_[* GetRemoteUrl]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 remoten
ame], [_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 url])&]
[s2; Returns the URL of the remote repository.&]
[s7; [*@3 remotename]-|Remote name&]
[s7;%% [%-*@3 url]-|Source string to store the remote URL&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetRemoteUrl`(const Upp`:`:String`&`): [_^Upp`:`:String^ String]_[* GetRemote
Url]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 remotename])&]
[s2; Returns the URL of the remote.&]
[s7; [*@3 remotename]-|Remote name&]
[s7; [*/ Return value]-|The URL of the remote&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetGitConfig`(bool`): [_^Git^ Git][@(0.0.255) `&]_[* GetGitConfig]([@(0.0.255) bo
ol]_[*@3 local])&]
[s2;%% Returns repository or global configuration options.&]
[s7; [*@3 local][%% -|][*@3 true] `- repository, [*@3 false] `- global configuration 
option&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetGitConfig`(bool`,const Upp`:`:String`&`,Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&
]_[* GetGitConfig]([@(0.0.255) bool]_[*@3 local], [@(0.0.255) const]_[_^Upp`:`:String^ Upp`:
:String][@(0.0.255) `&]_[*@3 name], [_^Upp`:`:String^ Upp`::String][@(0.0.255) `&]_[*@3 value
])&]
[s2; Get GIT configuration option value for given option name.&]
[s7; [*@3 local]-|[*@3 true] `- repository, [*@3 false] `- global configuration 
option&]
[s7; [*@3 name]-|Option name&]
[s7;%% [%-*@3 value]-|Option value&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:SetGitConfig`(bool`,char`,const Upp`:`:String`&`,const Upp`:`:String`&`): [_^Git^ G
it][@(0.0.255) `&]_[* SetGitConfig]([@(0.0.255) bool]_[*@3 local], [@(0.0.255) char]_[*@3 typ
e], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 name], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 value])&]
[s2; Set GIT configuration option.&]
[s7; [*@3 local]-|[*@3 true] `- repository, [*@3 false] `- global configuration 
option&]
[s7; [*@3 type]-|[*@3 A] (Add), [*@3 U] (Unset/Delete), [*@3 S] (Set) the 
option&]
[s7; [*@3 name]-|Option name&]
[s7; [*@3 value]-|Option value&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetDiff`(int`): [_^Git^ Git][@(0.0.255) `&]_[* GetDiff]([@(0.0.255) int]_[*@3 type])
&]
[s2; Returns changes in the working tree not yet staged for the next 
commit [*@3 (type 0)], changes between the index and your last 
commit; what you would be committing if you run `"git commit`" 
without `"`-a`" option [*@3 (type 1)] or changes in the working 
tree since your last commit; what you would be committing if 
you run `"git commit `-a`" [*@3 (type 2)].&]
[s7; [*@3 type]-|[*@3 0] `- DIFF`_TREE, [*@3 1] `- DIFF`_INDEX, [*@3 2] `- 
DIFF`_HEAD&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetDiff`(const Upp`:`:String`&`,const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&
]_[* GetDiff]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 oldversion],
 [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 newversion])&]
[s2; Returns changes between the [*@3 old`_version] and the [*@3 new`_version] 
of the branches.&]
[s7; [*@3 oldversion]-|Branch name&]
[s7; [*@3 newversion]-|Branch name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetDiff`(const Upp`:`:String`&`,bool`): [_^Git^ Git][@(0.0.255) `&]_[* GetDiff](
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 hash], 
[@(0.0.255) bool]_[*@3 files])&]
[s2; Returns changes between commit and its parent commit in the 
diff format.&]
[s7; [*@3 hash]-|Hash of the commit&]
[s7;%% [%-*@3 files]-|[*@3 true] `- return only file names and the nature 
of change, but not actual diff output&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetPatch`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* GetPatch]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 hash])&]
[s0;l288; Prepare commit with its patch in a file formatted to resemble 
UNIX mailbox format (with a fixed `"magic`" time stamp to indicate 
that the file is output from format`-patch rather than a real 
mailbox). The output of this command is convenient for e`-mail 
submission or for use with the command `'[*/ git am]`'.&]
[s7; [*@3 hash]-|Hash of the commit&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:PatchApply`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* PatchApply](
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 filepath])&]
[s2;%% Splits GIT patch file [*@3 filepath] into commit log message, 
authorship information and patches, and applies them to the current 
branch.&]
[s7; [*@3 filepath]-|GIT patch file name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:PatchAbort`(`): [_^Git^ Git][@(0.0.255) `&]_[* PatchAbort]()&]
[s2;%% Restore the original branch and abort the patching operation.&]
[s3; &]
[s4; &]
[s5;:Git`:`:IsPatchApplyPending`(`): [@(0.0.255) bool]_[* IsPatchApplyPending]()&]
[s2;%% Determine if there is the pending apply (e.g. after apply 
patch).&]
[s7; [*/ Return value]-|[*@3 true] `- there is the pending apply&]
[s3; &]
[s4; &]
[s5;:Git`:`:GetMergeDiff`(const Upp`:`:String`&`,const Upp`:`:String`&`,bool`): [_^Git^ G
it][@(0.0.255) `&]_[* GetMergeDiff]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 mergefrom], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 mergeto
], [@(0.0.255) bool]_[*@3 files])&]
[s2; Returns changes between branches for everything that is in the 
[*@3 mergeto] but not in the [*@3 mergefrom]. Used to display changes 
for the merge.&]
[s7; [*@3 mergefrom]-|Branch name&]
[s7; [*@3 mergeto]-|Branch name&]
[s7;%% [%-*@3 files]-|[*@3 true] `- return only file names, but not actual 
diff output&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:Merge`(const Upp`:`:String`&`,bool`): [_^Git^ Git][@(0.0.255) `&]_[* Merge]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 mergefrom], [@(0.0.255) bool]_[*@3 noff
])&]
[s2; Incorporates changes from the named commit [*@3 mergefrom] (since 
the time their histories diverged from the current branch) into 
the current branch.&]
[s7; [*@3 mergefrom]-|Branch name&]
[s7;%% [%-*@3 noff].-|[*@3 true] `- create a merge commit even when the 
merge resolves as a fast`-forward&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:IsMergePending`(`): [@(0.0.255) bool]_[* IsMergePending]()&]
[s2;%% Determine if there is the pending merge (e.g. after failed 
merge).&]
[s7; [*/ Return value]-|[*@3 true] `- there is the pending merge&]
[s3; &]
[s4; &]
[s5;:Git`:`:GetMergeMessage`(Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* GetMergeMess
age]([_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 message])&]
[s2;%% Returns merge message from .git/MERGE`_MSG file.&]
[s7;%% [%-*@3 message][%- -|]Source string to store merge message&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetMergeMessage`(`): [_^Upp`:`:String^ String]_[* GetMergeMessage]()&]
[s2;%% Returns merge message from .git/MERGE`_MSG file.&]
[s3; &]
[s4; &]
[s5;:Git`:`:GetUnmergedFiles`(Upp`:`:Vector`<Upp`:`:String`>`&`): [_^Git^ Git][@(0.0.255) `&
]_[* GetUnmergedFiles]([_^Upp`:`:Vector^ Vector]<[_^Upp`:`:String^ String]>`&_[*@3 fileli
st])&]
[s2;%% Returns list of unmerges files.&]
[s7;%% [%-*@3 filelist][%- -|vector of s]ource string to store file names&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:IsUnmergedFiles`(`): [@(0.0.255) bool]_[* IsUnmergedFiles]()&]
[s2;%% Determine if there are unmerged files in the working directory.&]
[s3; &]
[s4; &]
[s5;:Git`:`:Stash`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* Stash]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 message])&]
[s2; Save your local modifications to a new stash, and run git reset 
`-`-hard to revert them. The [*@3 message] part is optional and 
gives the description along with the stashed state.&]
[s7; [*@3 message]-|Description of the saved modification&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:StashApply`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* StashApply](
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 stashname])&]
[s2; Remove a single stashed state [*@3 stashname] from the stash list 
and apply it on top of the current working tree state, then tries 
to reinstate not only the working tree’s changes, but also 
the index’s ones.&]
[s7; [*@3 stashname]-|Name of the stashed state&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:StashList`(`): [_^Git^ Git][@(0.0.255) `&]_[* StashList]()&]
[s2; Returns the list the stashes that you currently have.&]
[s3; &]
[s4; &]
[s5;:Git`:`:IsStashList`(`): [@(0.0.255) bool]_[* IsStashList]()&]
[s2;%% Determine if there are stashed changes of the working directory 
and the index.&]
[s2; [*/ Return value]-|[*@3 true] `- there are stashed changes&]
[s3; &]
[s4; &]
[s5;:Git`:`:Reset`(const Upp`:`:Vector`<Upp`:`:String`>`&`,bool`): [_^Git^ Git][@(0.0.255) `&
]_[* Reset]([@(0.0.255) const]_[_^Upp`:`:Vector^ Vector]<[_^Upp`:`:String^ String]>`&_[*@3 f
ilelist], [@(0.0.255) bool]_[*@3 head])&]
[s2; Reset current HEAD to the specified state. Used to reset unmerged 
changes or cancel deletion of the file(s).&]
[s7; [*@3 filelist]-|List of the files&]
[s7; [*@3 head]-|[*@3 true] `- cancel deletion of the file(s)&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetLog`(bool`): [_^Git^ Git][@(0.0.255) `&]_[* GetLog]([@(0.0.255) bool]_[*@3 first
parent])&]
[s2; Returns the commit log.&]
[s7; [*@3 firstparent]-|[*@3 true] `- follow only the first parent commit 
upon seeing a merge commit&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetFileLog`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* GetFileLog](
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 filepath])&]
[s2; Returns the information about GIT history of given file name.&]
[s7;%% [%-*@3 filepath][%- -|]File name to view its GIT history&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetHashLog`(`): [_^Git^ Git][@(0.0.255) `&]_[* GetHashLog]()&]
[s2; Returns the commit hashes and commit messages.&]
[s3; &]
[s4; &]
[s5;:Git`:`:Blame`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* Blame]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 filepath])&]
[s2; Annotates each line in the given file with information from 
the revision with last modified the line.&]
[s7;%% [%-*@3 filepath][%- -|]File name to view its lines GIT history&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:TagAdd`(const Upp`:`:String`&`,const Upp`:`:String`&`,const Upp`:`:String`&`,bool`): [_^Git^ G
it][@(0.0.255) `&]_[* TagAdd]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 h
ash], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 tagname], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 message], 
[@(0.0.255) bool]_[*@3 force])&]
[s2; Creates an unsigned, annotated tag object [*@3 tagname] with the 
given tag [*@3 message]. [*@3 hash] is the object that the new tag 
will refer to.&]
[s2; The tag is specific points in history as being important. Typically 
people use this functionality to mark release points (v1.0, and 
so on).&]
[s7; [*@3 hash]-|commit`-id&]
[s7; [*@3 tagname]-|Tag name&]
[s7; [*@3 message]-|Tag message `- it can`'t be omited&]
[s7; [*@3 force]-|[*@3 true] `- replace an existing tag with the given 
name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:TagDelete`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* TagDelete]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 tagname])&]
[s2; Delete the tag object [*@3 tagname].&]
[s7; [*@3 tagname]-|Tag name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:TagGetDesc`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* TagGetDesc](
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 tagname])&]
[s2; Returns the tag message for the given [*@3 tagname].&]
[s7; [*@3 tagname]-|Tag name&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:GetConfigChangeType`(Upp`:`:String`&`,Upp`:`:String`&`): [@(0.0.255) char]_
[* GetConfigChangeType]([_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 oldvalue], 
[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 newvalue])&]
[s2; Returns type of the configuration option `- [*@3 A] (Add), [*@3 U] 
(Unset/Delete), [*@3 S] (Set).&]
[s7; [*@3 oldvalue]-|Old param value&]
[s7; [*@3 newvalue]-|New param value&]
[s7; [*/ Return value]-|Type of configuration&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:ProcessStashApply`(`): [_^Git^ Git][@(0.0.255) `&]_[* ProcessStashApply]()&]
[s2; Opens TopWindow to select and apply stashed changes on top of 
the current working tree state.&]
[s3; &]
[s4; &]
[s5;:Git`:`:ProcessBranchAdd`(const Upp`:`:String`&`): [_^Git^ Git][@(0.0.255) `&]_[* Proce
ssBranchAdd]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 startpoint])
&]
[s2; Opens TopWindow to create the new branch.&]
[s7; [*@3 startpoint]-|The new branch head will point to this commit 
(optional)&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:ProcessUpdateStatus`(const Upp`:`:Vector`<Upp`:`:String`>`&`): [@(0.0.255) v
oid]_[* ProcessUpdateStatus]([@(0.0.255) const]_[_^Upp`:`:Vector^ Vector]<[_^Upp`:`:String^ S
tring]>`&_[*@3 gitdirs])&]
[s2; [%% Opens TopWindow to show update status of the GIT repository(repositories). 
It is mainly designed to show ]difference status between local 
and remote branches for all local branches.&]
[s7; [*@3 gitdirs]-|GIT directories to check update status&]
[s3;%% &]
[s4; &]
[s5;:Git`:`:ProcessUpdateStatus`(`): [@(0.0.255) void]_[* ProcessUpdateStatus]()&]
[s2;%% Calls [* ProcessUpdateStatus] function with empty [*@3 gitdirs] 
parameter. This can be used to show update status of the actual 
GIT working directory.&]
[s3; &]
[s4; &]
[s5;:Git`:`:ProcessCommandOutput`(`): [@(0.0.255) virtual void]_[* ProcessCommandOutput](
)&]
[s2; Opens TopWindow to display strings from the last GIT command 
output.&]
[s3; &]
[s4; &]
[s5;:Git`:`:EditCommitMessage`(const Upp`:`:String`&`,const Upp`:`:Vector`<Upp`:`:String`>`&`,Upp`:`:String`&`): [@(0.0.255) i
nt]_[* EditCommitMessage]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 m
essage], [@(0.0.255) const]_[_^Upp`:`:Vector^ Vector]<[_^Upp`:`:String^ String]>`&_[*@3 p
arameters], [_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 msgpath])&]
[s2; Generate and edit commit message for the commit command. Commit 
[*@3 parameters] should be same as for the commit command to get 
appropriate message content. Commit message is stored to the 
file [*@3 msgpath].&]
[s7; [*@3 message]-|beginning of the commit message&]
[s7; [*@3 parameters]-|commit parameters&]
[s7; [*@3 msgpath]-|path to the file with the commit message&]
[s7; [*/ Return value]-|Exit code of the command execution&]
[s3;%% ]]