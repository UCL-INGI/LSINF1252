/* Header du fichier pour la résolution de l'exercice my-sem
Copyright (C) 2016 Dubray Alexandre

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

typedef struct semProcess {
    pthread_mutex_t mutex;
    struct semProcess *next;
} sem_process_t;

typedef struct mySem {
    int value;
    int capacity;
    sem_process_t *blocked_procs;
    pthread_mutex_t mutex;
} mysem_t;

int mysem_init(mysem_t *sem, unsigned int value);
int mysem_wait(mysem_t *sem);
int mysem_post(mysem_t *sem);
int mysem_close(mysem_t *sem);
