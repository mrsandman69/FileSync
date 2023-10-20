#/bin/bash

echo "Non-recursive sync test"
echo "____________________________"
#
rm -rf dir1
#
mkdir dir1
date > dir1/file1
cal  > dir1/file2
#
mkdir dir1/dir2
ls   > dir1/dir2/file3
#
echo "before syncing:"
tree dir1
tree dir1/dir2
echo "____________________________"
#
./mysync dir1 dir1/dir2
echo "after syncing:"
tree dir1
tree dir1/dir2
#
rm -rf dir1

echo "Recursive sync test"
echo "____________________________"
#
rm -rf dir1
#
mkdir dir1
date > dir1/file1
cal  > dir1/file2
#
mkdir dir1/dir2
ls   > dir1/dir2/file3
#
echo "before syncing:"
tree dir1
tree dir1/dir2
echo "____________________________"
#
./mysync -r dir1 dir1/dir2
echo "after syncing:"
tree dir1
tree dir1/dir2
#
rm -rf dir1
