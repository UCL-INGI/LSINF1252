# Documentation CTester

CTester est une librairie permettant d'écrire simplement et rapidement des tests INGInious pour des exercices en C. Elle est basée sur CUnit et offre certaines fonctionnalités pratiques :

 - Sandboxing du code de l'étudiant (segfaults, timeout, *double free*)
 - Buffers "piégés" (read-only et mémoire adjacente protégée, mémoire allouée par malloc initialement non nulle)
 - Retour d'info à l'étudiant via INGInious
 - Statistiques d'utilisation et interception de certains appels systèmes
- Interdiction d'utiliser des fonctions spécifiques
-   Support de l'internationalisation via gettext 

## Introduction

Toute suite de tests pour un exercice C utilisant CTester respecte l'arborescence suivante :

 - *run* : script Python, interface entre INGInious et CTester
 - *student/*
	 - *Makefile*
	 - *tests.c* : les tests de l'exercice
	 - *student_code.c.tpl* : template dans lequel le code de l'étudiant sera injecté
	 - *student_code.h* : header reprenant les fonctions à tester
	 - *CTester/*
		 - ... (code de la librairie)

Lors de la réalisation d'un exercice, les seuls fichiers à modifier sont *tests.c*, *student_code.c.tpl* et *student_code.h*.

Voici un exemple de test CTester pour la fonction `insert`, lorsque le fichier indiqué n'existe pas :

```c
/*
* @pre fileName!=NULL, buf!=NULL, nbyte >0
* @post a inséré dans le fichier filename les nbytes du buffers buf
*       à la position pos. Les bytes suivants dans le fichiers ont
*       été décalés vers la fin du fichier.
*       Retourne le nombre de bytes écrits, -1 en cas d'erreur
*/
int insert(char *fileName, off_t pos, const void *buf, size_t nbyte);
```
```c
void test_insert_no_file();
	set_test_metadata("insert", _("If the given file doesn't exist, return -1"), 1);
	int ret = -1000;
	char *c="XYZ";

	monitored.open=true;
	SANDBOX_BEGIN;
	ret=insert("fkjkj.dat",-5,c,strlen(c));
	SANDBOX_END;

	CU_ASSERT_EQUAL(ret,-1);
	CU_ASSERT_EQUAL(stats.open.called, 1);
	if (stats.open.called > 1)
	    push_info_msg(_("Why did you use open more than once ?"));
}
```

Un test CTester est une simple fonction sans argument et sans valeur de retour. Au début du test, il est important de lui assigner certaines meta-informations qui seront renvoyées à INGInious via `void set_test_metadata(char *problem, char *descr, unsigned int weight);` :
- *problem* est une chaîne de caractères, correspondant au problème de la tâche INGInious liée que le test vérifie (un problème sur INGInious correspond à une boite de dialogue où on peut entrer du code, une tâche correspond à un exercice, qui peut reprendre plusieurs problèmes) ;
- *descr* contient un bref descriptif de ce que le test vérifie ;
- *weight* représente la pondération du test pour la tâche (par ex. si weight=2, le test vaut 2 points).

Lorsqu'on veut faire appel au code de l'étudiant, il est **OBLIGATOIRE** de le faire depuis la [*sandbox*](https://fr.wikipedia.org/wiki/Sandbox_%28s%C3%A9curit%C3%A9_informatique%29), en utilisant les macros `SANDBOX_BEGIN` et `SANDBOX_END`. La *sandbox* permet d'éviter qu'un *segfault* ou une boucle infinie dans le code de l'étudiant ne fasse planter toute la suite de tests. De même, les fonctionnalités de monitoring d'appels systèmes ne fonctionnent qu'à l'intérieur de la *sandbox*. Il est important de préciser que le code à l'intérieur de celle-ci est capable de crasher à tout moment, propulsant alors l'exécution du programme à ce qui suit `SANDBOX_END`.  Dès lors, si vous souhaitez utiliser des variables dans vos assertions à la fin du test, il faut déclarer celles-ci en dehors de la *sandbox* (comme `ret` dans l'exemple).

Tous les types d'assertions de CUnit sont disponibles dans CTester, se référer à [la documentation de CUnit](http://cunit.sourceforge.net/doc/writing_tests.html). La fonction `push_info_msg` permet d'indiquer un message supplémentaire à l'étudiant, pour l'aider à corriger son code. CTester rapporte à l'étudiant automatiquement un éventuel *segfault*, *timeout* ou *double free*. On peut pousser autant de messages que l'on souhaite, mais le framework interdit l'usage du caractère '#' ou d'un retour à la ligne dans les messages.

Finalement, afin de de permettre de traduire les suites de tests, il est également important d'appliquer *gettext* à toutes vos chaînes de caractères via la macro `_` : `_("My string")`. La possibilité de traduire ces chaînes en français est expliquée dans la section "Internationalisation".

## Statistiques et interception d'appels systèmes

Il est possible de récupérer des statistiques d'utilisation et d'intercepter certains appels systèmes utilisés par le code de l'étudiant.

Les appels systèmes interceptables sont :
* *wrap_getpid.h* : getpid
* *wrap_file.h* : open, creat, close, read, write, stat, fstat, lseek
* *wrap_malloc.h* : malloc, calloc, realloc, free
* *wrap_mutex.h* : pthread_mutex_lock, pthread_mutex_trylock, pthread_mutex_unlock, pthread_mutex_init, pthread_mutex_destroy

Afin d'activer la génération de statistiques ou l'interception pour un appel système, il faut utiliser la variable globale `monitoring`, chaque appel dispose d'un booléen pour activer son monitoring : `monitoring.open = true;`. 

**Attention**, la génération de statistiques et l'interception d'appels systèmes ne fonctionne qu'à l'intérieur de la *sandbox* ! Il n'est pas nécessaire de remettre tous les membres de `monitoring` à 0 entre les tests, ceci est fait automatiquement par CTester (ceci vaut également pour les variables `stats`, `failures` et `logs` introduites ci-après).

### Statistiques d'appels
Une fois le monitoring d'un appel système activé, CTester récupère automatiquement certaines statistiques de son utilisation :

```c
void test_pop()
{
	set_test_metadata("pop", _("Free the item when using pop"), 1);
	struct LL *linked_list = ...;
	void *last_item = (void *) linked_list->last;

	monitored.free=true;
	SANDBOX_BEGIN;
	pop(linked_list);
	SANDBOX_END;

	CU_ASSERT_EQUAL(stats.free.called, 1);
	CU_ASSERT_EQUAL(stats.free.last_params.ptr, last_item);
}
```

Tous les appels systèmes enregistrent le nombre d'appels (`stats.FUNC.called`), le dernier ensemble d'arguments utilisés (`stats.FUNC.last_params.ARG`, se référer aux fichiers header cités ci-dessus pour les noms des arguments de chaque appel), et l'éventuelle dernière valeur de retour (`stats.FUNC.last_return`). Pour des appels systèmes modifiant un buffer, celui-ci est également enregistré (voir par exemple `fstat`).

### Interception d'appels

Il est possible de faire échouer un appel système en forçant sa valeur de retour via la variable globale `failures` : `failures.FUNC = PATTERN`, où `PATTERN` est un entier non signé sur 32 bits, le $N$ième bit indiquant si le $N$ième appel à `FUNC` doit échouer (en démarrant du bit de poids faible).  

Par exemple, `failures.malloc = 0b00000000000000000000000000000101` fera échouer le 1er et 3ème appel à `malloc`.  Des constantes de pattern sont disponibles : `FAIL_ALWAYS`,`FAIL_NEVER`, `FAIL_FIRST`, `FAIL_SECOND`, `FAIL_THIRD`, `FAIL_TWICE` (pour faire échouer respectivement, toujours, jamais, le premier appel, le second, le troisième, les deux premiers).

Selon le prototype de l'appel système, il est également possible d'indiquer la valeur de retour et la valeur d'`errno` à renvoyer lorsque l'appel échoue, respectivement via `failures.FUNC_ret` et `failures.FUNC_errno` (voir *CTester/wrap.h* pour plus de détails).

```c
void test_write_fail() {
	set_test_metadata("insert", "Utilisation de write: en cas d'erreur, retourne -1", 1);
	char *c="XYZ";
	system("echo -n ABCDEF > f.dat");
	int ret = -1000;

	SANDBOX_BEGIN;
	ret = insert("f.dat",3,c,strlen(c));
	SANDBOX_END;
	CU_ASSERT_EQUAL(ret, 0);

	ret = -1000;
	monitored.write = true;
	failures.write=FAIL_ONCE;
	failures.write_ret=-1;
	failures.write_errno=EIO;

	SANDBOX_BEGIN;
	ret=insert("f.dat",1,c,strlen(c));
	SANDBOX_END; 
	CU_ASSERT_EQUAL(ret,-1);

	system("rm f.dat");
}
```


### Accès aux logs de malloc
Finalement, lorsque le monitoring de `malloc` a été activé, on peut utiliser les fonctions suivantes :

```c
/*
* returns true if the address has been managed by malloc, false
* otherwise (also false if address has been freed)
*/
int malloced(void *addr);

/*
* returns total amount of memory allocated by malloc
*/
int  malloc_allocated();
```

A noter également que `malloc` a été configuré (via `mallopt`) de façon à ce que toute mémoire allouée est garantie de ne pas être initialisée à 0.

## Buffers "piégés"

On peut partiellement vérifier que l'étudiant ne fait pas de [*buffer overflow*](https://fr.wikipedia.org/wiki/D%C3%A9passement_de_tampon) à l'aide de la fonction `trap_buffer`  :

```c
/* @size: buffer's size
   @type: TRAP_LEFT or TRAP_RIGHT (location of adjacent protected page)
   @flags : permissions: OR on subset of (PROTO_READ, PROTO_WRITE, PROTO_NONE)
   @data : fill buffer with initial data if != NULL

   Return: pointer to created buffer on the heap
*/
void *trap_buffer(size_t size, int type, int flags, void *data);
```

`trap_buffer` alloue un buffer, avec une page mémoire protégée (`PROTO_NONE`, ni lecture, ni écriture autorisées) adjacente à sa gauche ou à sa droite. Si l'étudiant dépasse la taille allouée du buffer du coté indiqué, ou tente d'écrire dans un *buffer* en lecture seule, un SEGFAULT sera généré.

Il est conseillé de "piéger" tous les buffers passés aux fonctions à tester.

## Interdiction de fonctions

On peut interdire l'utilisation d'une fonction de la librairie standard à l'étudiant en insérant quelque part dans *tests.c* l'annotation `BAN_FUNCS(...)`. Celle-ci peut être insérée dans un commentaire ou directement dans le code (une macro a été prévue à cet effet) :

```c
int main(int argc,char** argv)
{
	BAN_FUNCS(strlen, strcat);
	RUN(test_mystrlen_1, test_mystrlen_2, test_mystrcat_1, test_mystrcat_2);
}
```

Deux points sont à souligner :

 - La chaîne  `BAN_FUNCS(...)` ne doit être présente qu'une seule fois dans le code source (seule sa dernière occurrence est prise en compte).
 - L'interdiction s'applique à tous les problèmes, c'est-à-dire toutes les fonctions qui seront injectées dans `student_code.c.tpl`. Il n'est pas possible d'appliquer une interdiction à un problème spécifique.

## Internationalisation

Les chaînes de caractère passées à `gettext` seront traduites automatiquement par INGInious selon la langue de l'utilisateur. Il faut néanmoins pour ce faire rédiger les traductions des chaînes, des actions ont été ajoutées au `Makefile` pour faciliter cette étape :

 1. Lors de la création d'un nouvel exercice, une fois les tests écrits, utilisez `make create-po`. Cela créera un dossier `po/`. Par défaut, seul le sous-dossier `po/fr/` est créé afin de pouvoir traduire en français la suite de tests.
 2. Insérez les traductions dans le fichier `po/fr/tests.po`.
 3. Compilez les traductions via `make compile-mo`, vous pouvez dès forcer la traduction en français en utilisant la variable d'environnement `LANG` : `LANG=fr_BE.utf8 ./tests && cat results.txt` (attention d'assigner à `LANG` une locale installée sur votre système et répertoriée par `locale -a`).
 4. Si vous avez modifié à posteriori une chaîne de caractères dans un fichier C, afin de l'inclure dans `tests.po`, utilisez `make update-po` au lieu de `make create-po`, sinon vos traductions précédentes seront perdues.

