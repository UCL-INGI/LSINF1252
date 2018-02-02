#! /bin/bash

MAIN_POT="\$i18n/messages.pot";
C_POT="\$i18n/c.pot";
TASKS_POT="\$i18n/tasks.pot"

echo '[1] Generating .pot file from C sources, run script, and INGInious tasks files';

# We list all .c and .c.tpl files
C_LIST=$(find . -name "*.c*" -exec echo -n '{} ' \; | tr '\n' ' ');

#Generate the main .pot file
xgettext -k_ --from-code UTF-8 -o $C_POT $C_LIST;

#Generate the tasks .pot file
pybabel extract -F \$i18n/babel.cfg -o $TASKS_POT .;

#Merge both .pot files into one
msgcat $C_POT $TASKS_POT > $MAIN_POT;

git add $MAIN_POT;

# Update all .po file from .pot to stay consistent with the Strings present in .java sources.
echo '[2] Updating .po file from .pot';
shopt -s nullglob;
for i in $\i18n/*.po; do
    msgmerge -vU $i $MAIN_POT 2>/dev/null;
    git add $i;
done

git commit -m "Auto commit: updating .pot and .po files";
git push;h
