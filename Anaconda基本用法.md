# Anaconda基本用法

## 进入操作

- $ python	: 进入Python交互界面
- $ anaconda-navigator    : 启动Anaconda-Navigator		 

## 创建虚拟环境

1. 创建新环境

   ```
   conda create --name <env_name> <package_names>
   ```

2. 激活环境

   ```
   source activate  <env_name>
   ```

3. 退出环境

   ```
   conda deactivate
   ```



## 查询、复制、删除、安装、卸载、更新操作

- $ conda info -e    :  显示已创建的环境

- > 复制：
  >
  > ```
  > conda create --name <new_env_name> --clone <copied_env_name>
  > ```

- > 删除环境
  >
  > ```
  > conda remove --name <env_name> --all
  > ```

- $ conda list    : 获取当前环境中已安装的包信息

- > 查找可供安装的包版本
  >
  > - 精确查找：
  >
  >   ```
  >   conda search --full-name <package_full_name>
  >   ```
  >
  > - 模糊查找：
  >
  >   ```
  >   conda search <text>
  >   ```

- > 安装包
  >
  > - 在指定环境中安装包
  >
  >   ```
  >   conda install --name <env_name> <package_name>
  >   ```
  >
  > - 在当前环境中安装包
  >
  >   ```
  >   conda install <package_name>
  >   ```

- > 卸载包
  >
  > - 卸载指定环境中的包
  >
  >   ```
  >   conda remove --name <env_name> <package_name>
  >   ```
  >
  > - 卸载当前环境中的包
  >
  >   ```
  >   conda remove <package_name>
  >   ```

- > 更新包
  >
  > - 更新所有包
  >
  >   ```
  >   conda update --all
  >   ```
  >
  >   或
  >
  >   ```
  >   conda upgrade --all
  >   ```
  >
  > - 更新指定包
  >
  >   ```
  >   conda update <package_name>
  >   ```
  >
  >   或
  >
  >   ```
  >   conda upgrade <package_name>
  >   ```

