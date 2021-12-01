rm -f replace.vim

for f in object
do
  echo "gen $f"
  awk '{print ":%s/\<"$1"\>/tk_"$1"/g"}' $f.txt >> replace.vim 
  echo "#ifndef USE_TK_PREFIX" >$f"_compat.h"
  awk '{print "#define " $1 " tk_"$1 }' $f.txt >>$f"_compat.h"
  echo "#endif/*USE_TK_PREFIX*/" >>$f"_compat.h"
done

echo ":wq" >>replace.vim
echo "done"
