
for f in tools/rename/*.vim;
do
  find src -name \*.c -exec vim -s $f {} \;
  find src -name \*.h -exec vim -s $f {} \;
  find src -name \*.inc -exec vim -s $f {} \;
  find tests -name \*.c -exec vim -s $f {} \;
  find tests -name \*.cc -exec vim -s $f {} \;
  find tests -name \*.cpp -exec vim -s $f {} \;
done
