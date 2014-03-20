rm -rf $2

~/gcc_build/install/bin/gcc -flto -flto-partition=none -fplugin=./plugin.so -fdump-tree-all -O2 -fdump-ipa-pta -fipa-pta $1 > out.lipta

mkdir -p $2/lipta $2/out
mv $1.* $2/lipta
mv out.lipta $2/out

~/gcc_build/install/bin/gcc -fdump-tree-all -O2 -fdump-ipa-pta -fipa-pta $1 > out.gcc

mv $1.* $2
mv out.gcc $2/out

cat $2/out/out.lipta

#echo "Diff::"

# diff $2/lipta/$1.149t.optimized $2/lipta/$1.149t.optimized

cd $2
for file in `ls $1.*`; do echo "**********$file************"; diff $file lipta/$file; done > out/diff.out
cd -

echo "############################################  Diff  ############################################"

cat $2/out/diff.out
grep SUCCESS $2/out/out.lipta
