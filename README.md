"# stm32F407ZGT6_hal" 


把这个目录变成Git可以管理的仓库
git init
将所有文件添加到仓库
git add .
把文件提交到仓库，双引号内是提交注释。
git commit -m "updata"
关联github仓库，“ssh”代表你仓库的ssh地址
git remote add origin “ssh”
上传本地代码
git push -u origin master
完成后，没有报错代表上传完成，本地代码已经推送到github仓库了，我们现在去githubt仓库看看

For test



拉取远程仓库git pull origin master --allow-unrelated-histories
git add .“.” 的意思是所有文件，也可以指定文件
提交说明 git commit -m "updata"
更新GitHub远程仓库git push -u origin master
