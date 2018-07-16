import os, re, hashlib, subprocess, shlex, shutil
import tempfile, tarfile
from inginious import feedback, rst, input

#################
# Check filename
#################

fname = input.get_input("p1archive:filename")

if re.match(r"^[0-9]{6}00\.tar\.gz$", fname) is None:
    feedback.set_global_result("failed")
    feedback.set_global_feedback("Nom de l'archive incorrect. Format attendu: NOMA.tar.gz (exemple: 12341500.tar.gz)")
    exit(1)

noma = fname.replace(".tar.gz", "")

##################
# Extracting file
##################

fp = tempfile.TemporaryFile()
fp.write(input.get_input("p1archive:value"))
fp.seek(0)

try:
    tar = tarfile.open(fileobj=fp)
    tar.extractall()
    tar.close()
except:
    feedback.set_global_result("failed")
    feedback.set_global_feedback("Erreur lors de la décompression de l'archive. Vérifiez qu'elle est bien au format .tar.gz.")
    exit(1)
    
fp.close()

################
# Content check
################

if not os.path.exists(noma):
    feedback.set_global_result("failed")
    feedback.set_global_feedback("Répertoire attendu inexistant. Au vu du nom du fichier, le répertoire devrait s'appeler: " + noma)
    exit(1)

if not os.path.isdir(noma):
    feedback.set_global_result("failed")
    feedback.set_global_feedback(noma + " n'est pas un répertoire.")
    exit(1)
    
files = ["matrix.h", "matrix.c", "test.c", "report.pdf", "Makefile"]    

for f in files:
    if not os.path.exists(noma + "/" + f):
        feedback.set_global_result("failed")
        feedback.set_global_feedback("Fichier " + f + " non trouvé")
        exit(1)   

exp_sha256 = "ee7d70090b87564988c11eec31b2578932725faed9dc7a941fed2e6552cdb466"
obt_sha256 = hashlib.sha256(open(noma + "/matrix.h", 'rb').read()).hexdigest()

if not obt_sha256 == exp_sha256:
    feedback.set_global_result("failed")
    feedback.set_global_feedback("Le fichier matrix.h a été modifié")
    exit(1) 

remaining_files = set(os.listdir(noma)) - set(files)
remfile_count = len(remaining_files)

#if remfile_count > 0:
#    feedback.set_global_result("failed")
#    feedback.set_global_feedback("Il existe " + str(remfile_count) + " fichiers inconnus dans l'archive, veuillez les retirer: \n\n")
#    for f in remaining_files:
#    	feedback.set_global_feedback("- " + f + "\n", True)
#    exit(1) 

p = subprocess.Popen(shlex.split("gcc -std=gnu99 -o " + noma + "/test " + noma + "/matrix.c " + noma + "/test.c -lcunit"), stderr=subprocess.STDOUT, stdout=subprocess.PIPE)
gcc_output = p.communicate()[0].decode('utf-8')
# If compilation failed, exit with "failed" result
if p.returncode:
    feedback.set_global_result("failed")
    feedback.set_global_feedback("La compilation de votre code a échoué. Voici le message de sortie de la commande :")
    feedback.set_global_feedback(rst.get_codeblock('', gcc_output), True)
    exit(0)
else:
    feedback.set_global_result("success")
    feedback.set_global_feedback("- Votre code compile.\n")    

os.remove(noma + "/test")
os.chdir(noma)

p = subprocess.Popen(shlex.split("make"), stderr=subprocess.STDOUT, stdout=subprocess.PIPE)
make_output = p.communicate()[0].decode('utf-8')
# If compilation failed, exit with "failed" result
if p.returncode:
    feedback.set_global_result("failed")
    feedback.set_global_feedback("Impossible d'exécuter le Makefile. Voici le message de sortie de la commande :", True)
    feedback.set_global_feedback(rst.get_codeblock('', make_output), True)
    exit(0)
else:
    if os.path.exists("test"):
        feedback.set_global_result("success")
        feedback.set_global_feedback("- Votre Makefile fonctionne.\n", True)
    else:
        feedback.set_global_result("failed")
        feedback.set_global_feedback("- La commande make n'a pas généré d'exécutable nommé test.", True)
        exit(0)

os.chdir("..")
shutil.copyfile(noma + "/matrix.c", "student/matrix.c")
shutil.copyfile(noma + "/matrix.h", "student/matrix.h")
feedback.set_global_result("success")