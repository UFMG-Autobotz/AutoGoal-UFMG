# Scripts to rule them all

These scripts are inspired by the [normalized script pattern that GitHub uses in its projects](https://githubengineering.com/scripts-to-rule-them-all/).
Here's a description of what each script used in this project does.


## Script/setup
> sets up a project to be used for the first time

Use it once after cloning the project to:
* create useful aliases;
* download submodules;
* prepare submodules to allow your contribution.


## Script/update
> updates a project to run at its current version

Use it after `git pull` to:
* update submodules;
* recompile all plugins of the Autobotz-Simulator submodule.
