#! /bin/bash

echo "[3] Generating .mo files"

shopt -s nullglob;

for i in \$i18n/*.po; do

    # Extract the file name (without path and extension)
    filename=$(basename "$i");
    extension="${filename##*.}";
    filename="${filename%.*}";

    #Generates the .mo file for PYTHON
    msgfmt $i -o \$i18n/$filename.mo
    git add \$i18n/$filename.mo

    # We copy all translations in all tasks
    for taskfile in */task.yaml; do
	j=$(dirname $taskfile);

	#for tasks
	mkdir -p $j/\$i18n/
	cp \$i18n/$filename.mo $j/\$i18n/$filename.mo
	git add $j/\$i18n/$filename.mo
    done
done

git commit -m "Update translations of all tasks"
git push
