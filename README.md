# Wireless-weather-station
Aalto University’s ELEC-8701 Elektronics workshop courses repository for Axel Soderberg, Roope Jaskari and Kalle Aro

# Basic git usage for this project:

Sections 1 to 2 are done on the command line
# 1. Initialization 

If you don't have an ssh key yet follow the instruction in [this link](https://www.howtogeek.com/762863/how-to-generate-ssh-keys-in-windows-10-and-windows-11/)
Add this ssh key to your GitHub SSH keys and proceed.

## clone project
```
git clone git@github.com:<yourusername>/Wireless-weather-station.git
```

Go to the cloned repository > "Wireless-weather-station" (default name)

## Make your own branch
```
git branch <your own branch name here>
```
```
git checkout <your own branch name here>
```
# 2. Working with git

When you want to commit something, first check out the changes you have made.
```
git status
```
After this add the files you'd like to commit

```
git add <your file/folder in the git initialized folder>
```
Commit
```
git commit -m "<your required message here>"
```
And if this is your first push to the git repository use this to also set your upstream:

```
git push --set-upstream origin
```
Otherwise:
```
git push
```

# 3. Merging with main
After a successful commit and push, if you'd like to begin a merge request with your branch and the main branch, navigate to your branch in GitHub from this dropdown:

<img width="375" alt="image" src="https://github.com/user-attachments/assets/db4c9d65-d420-4276-92b1-a5576ae31734">

And then make a pull request here: 

<img width="908" alt="Screenshot 2024-10-20 at 23 57 30" src="https://github.com/user-attachments/assets/269801a1-92e0-4108-aa97-69f968f60443">

After this, notify the other contributors about the pull request in order to get their approval on the matter. Success!

# In addition one can delete their own branch from the GitHub to start a new addition to the project from a clean table. 
# DO NOT TRY TO DELETE THE MAIN BRANCH!



