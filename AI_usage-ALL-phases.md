PHASE 1

1. Instrument utilizat

Pentru partea asistată de inteligență artificială am folosit ChatGPT, ca suport pentru generarea și analizarea unor funcții cerute în enunț.

2. Scopul utilizării AI

AI-ul a fost utilizat pentru implementarea a două funcții:

- parse_condition
- match_condition

3. Prompturile utilizate

Prompt 1 – Parsarea condițiilor

Lucrez la un program în C care implementează un sistem de filtrare pentru rapoarte stocate în fișiere binare.
Condițiile sunt primite din linia de comandă sub forma field:operator:value (de exemplu severity:>=:2 sau category:==:road).
Am nevoie de o funcție în C care primește acest string și separă cele trei componente (câmp, operator, valoare) în variabile 
diferite (char arrays). Fără alocări dinamice, folosind funcții din C. Funcția ar trebui să întoarcă un cod de succes/eșec dacă parsarea nu reușește.

Prompt 2 – Verificarea condițiilor

Am o structură report definită: severity (int), category (string), inspector (string), timestamp (time_t)
Vreau să imi scrii o funcție în C care verifică dacă un obiect report respectă o condiție de forma field:operator:value.
Operatorii sunt: ==, !=, <, <=, >, >=.
Trateaza diferența între tipuri: comparații numerice pentru severity și timestamp comparații de string pentru category și inspector.
Funcția trebuie să returneze 1 (true) si 0 (fals).

4. Codul generat inițial de AI

AI-ul a generat:

int parse_condition(char *input,char *field,char *op,char *value)
{
    return sscanf(input,"%[^:]:%[^:]:%s",field,op,value)==3;
}

int match_condition(report *r,char *field,char *op,char *value)
{
    if(strcmp(field,"severity")==0)
    {
        int v=atoi(value);
        if(!strcmp(op,"=="))
            return r->severity==v;
        if(!strcmp(op,"!="))
            return r->severity!=v;
        if(!strcmp(op,">"))
            return r->severity>v;
        if(!strcmp(op,"<"))
            return r->severity<v;
        if(!strcmp(op,">="))
            return r->severity>=v;
        if(!strcmp(op,"<="))
            return r->severity<=v;
    }
    if(strcmp(field,"category")==0)
    {
        if(!strcmp(op,"=="))
            return strcmp(r->category,value)==0;
        if(!strcmp(op,"!="))
            return strcmp(r->category,value)!=0;
    }
    if(strcmp(field,"inspector")==0)
    {
        if(!strcmp(op,"=="))
            return strcmp(r->inspector,value)==0;
    }
    if(strcmp(field,"timestamp")==0)
    {
        long v=atol(value);
        if(!strcmp(op,"=="))
            return r->timestamp==v;
        if(!strcmp(op,">"))
            return r->timestamp>v;
        if(!strcmp(op,"<"))
            return r->timestamp<v;
        if(!strcmp(op,">="))
            return r->timestamp>=v;
        if(!strcmp(op,"<="))
            return r->timestamp<=v;
    }
    return 0;
}

5. Probleme identificate

Codul generat pentru match_condition este deja destul de bun, însă repetitiv. Pentru timestamp si severity sunt folosite aceleasi comparații, și nu ar fi necesar sa duplicam codul. Asemena și pentru inspector și category.
De asemenea, pentru funcția parse_condition, codul generat nu este destul de robust, nu ia in calcul situațiile de parsing!=3, validare field, operator si valoare.

6. Îmbunătățiri aduse

Am modificat codul generat astfel:

int parse_condition(const char *input,char *field,char *op,char *value)
{
    if(sscanf(input,"%[^:]:%[^:]:%s",field,op,value)!=3)
        return 0;

    if(strcmp(field,"severity")!=0 &&
       strcmp(field,"category")!=0 &&
       strcmp(field,"inspector")!=0 &&
       strcmp(field,"timestamp")!=0)
        return 0;

    if(strcmp(op,"==")!=0 &&
       strcmp(op,"!=")!=0 &&
       strcmp(op,">")!=0 &&
       strcmp(op,"<")!=0 &&
       strcmp(op,">=")!=0 &&
       strcmp(op,"<=")!=0)
        return 0;

    return 1;
}

int cmp_int(int a,int b,const char *op)
{
    if(!strcmp(op,"=="))return a==b;
    if(!strcmp(op,"!="))return a!=b;
    if(!strcmp(op,">"))return a>b;
    if(!strcmp(op,"<"))return a<b;
    if(!strcmp(op,">="))return a>=b;
    if(!strcmp(op,"<="))return a<=b;
    return 0;
}

int cmp_str(const char *a,const char *b,const char *op)
{
    if(!strcmp(op,"=="))return strcmp(a,b)==0;
    if(!strcmp(op,"!="))return strcmp(a,b)!=0;
    return 0;
}

int match_condition(Report *r,const char *field,const char *op,const char *value)
{
    if(strcmp(field,"severity")==0)
        return cmp_int(r->severity,atoi(value),op);

    if(strcmp(field,"timestamp")==0)
        return cmp_int(r->timestamp,atol(value),op);

    if(strcmp(field,"category")==0)
        return cmp_str(r->category,value,op);

    if(strcmp(field,"inspector")==0)
        return cmp_str(r->inspector,value,op);

    return 0;
}

Pentru funcția parse am utilizat mai multe verificări, pentru mai multe tipuri de situații.
Pentru funcșia match am creat alte funcții auxiliare care să evite duplicarea codului.

7. Ce am învățat

Să analizez critic cod generat de AI
Validarea datelor de intrare
Integrarea unor funcții mici într-un sistem mai mare
Implementarea filtrării pe bază de condiții
Evitarea duplicării codului
Luarea în calcul a tuturor variantelor de testarw

8. Concluzie

AI-ul a fost util pentru generarea unei baze inițiale, dar a fost necesară intervenția mea pentru completarea logicii.




PHASE 2


1. Instrument utilizat

Pentru partea asistată de inteligență artificială am folosit ChatGPT, ca suport pentru generarea și analizarea unor funcții cerute în enunț.

2. Scopul utilizării AI

AI-ul a fost utilizat pentru a înțelege cum opereaza semnalele. De exemplu, am vrut sa stiu cum să tratez doua semnale in acelasi proram, daca pot sa le combin implementarea sau nu. 


3. Prompturile utilizate

Prompt 1 – Imi propun sa scriu un program in C ce necesita tratarea semnalului SIGUSR1(trimis dintr-un alt program C atunci cand fac operatia de adunare dintr-un alt program C) si SIGINT pentru atunci cand termin
din terminal executia. Cum as putea sa implementez acest lucru intr-un proiect? Ma intereseaza mainul.

Prompt 2 – Imi doresc o implementare fara sigemptyset.

4. Codul generat inițial de AI

AI-ul a generat la Prompt 1:

    void handle_sigusr1(int sig)
{
    suma += 5;

    printf("\n[SIGUSR1] S-a efectuat adunarea.\n");
    printf("Suma curenta: %d\n", suma);
}

void handle_sigint(int sig)
{
    printf("\n[SIGINT] Programul se inchide...\n");
    running = 0;
}

Iar în main:

    struct sigaction sa_usr1;
    struct sigaction sa_int;

    // Handler pentru SIGUSR1
    sa_usr1.sa_handler = handle_sigusr1;
    sigemptyset(&sa_usr1.sa_mask);
    sa_usr1.sa_flags = 0;

    sigaction(SIGUSR1, &sa_usr1, NULL);

    // Handler pentru SIGINT
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;

    sigaction(SIGINT, &sa_int, NULL);

AI-ul a generat la Prompt 2:

    struct sigaction sa_int, sa_usr1;

    memset(&sa_int, 0, sizeof(sa_int));
    sa_int.sa_handler = handle_sigint;
    sigaction(SIGINT, &sa_int, NULL);

    memset(&sa_usr1, 0, sizeof(sa_usr1));
    sa_usr1.sa_handler = handle_sigusr1;
    sigaction(SIGUSR1, &sa_usr1, NULL);


5. Probleme identificate

M-am gandit ulterior ca nu e necesar sa definesc doua handle-uri, as putea trata totul dintr-un singur handler. Am modificat acest lucru.

6. Îmbunătățiri aduse

Am modificat codul generat astfel:

void handler(int sig)
{
    if(sig==SIGINT)
    {
        printf("SIGINT recieved. Terminating.\n");
        unlink(".monitor_pid");
        exit(0);
    }
    else if(sig==SIGUSR1)
    {
        printf("SIGUSR1 recieved. Adding new report.\n");
    }
}

In main:

    struct sigaction signal;
    memset(&signal,0,sizeof(signal));
    signal.sa_handler=handler;
    sigaction(SIGINT,&signal,NULL);
    sigaction(SIGUSR1,&signal,NULL);


7. Ce am învățat

Să analizez critic cod generat de AI
Evitarea duplicării codului
Evitarea complicarii codului

8. Concluzie

AI-ul a fost util cu scop didactic, dar a fost necesară intervenția mea pentru a corecta logica.



PHASE 3

1. Instrument utilizat

Am continuat să folosesc ChatGPT ca suport.

2. Scopul utilizării AI

Am avut nevoie de ajutor în două zone:

Înțelegerea fluxului de pipe + dup2 pentru a redirecta stdout-ul monitorului prin pipe spre hub_mon
Implementarea funcției de cleanup pentru dangling symlinks, symlink-urile active_reports din Phase 1 pot deveni dangling (pointează spre fișiere șterse).

3. Prompturile utilizate

Prompt 1

Am un program C în care un proces părinte (hub_mon) trebuie să citească output-ul unui proces copil (monitor_reports). Vreau să folosesc un pipe și dup2 pentru a redirecta stdout-ul copilului spre pipe. Cum ar arăta structura de fork + pipe + dup2 + exec? Ce descriptori trebuie închiși și în ce ordine?

Prompt 2

În programul meu C am symlink-uri create cu symlink() care pointează spre fișiere reports.dat din directoare de districte. 
Dacă un district e șters (cu remove_district), symlink-ul rămâne dangling. 
Am nevoie de o funcție care parcurge directorul curent, identifică symlink-urile active_reports-* cu lstat(), verifică dacă sunt dangling, și le șterge cu unlink().

4. Codul generat inițial de AI

La Prompt 1, AI-ul a generat:

int fd[2];
pipe(fd);
pid_t pid = fork();
if(pid == 0) {
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);
    execl("./child_program", "child_program", NULL);
    exit(1);
}
close(fd[1]);
char buf[256];
int n;
while((n = read(fd[0], buf, sizeof(buf)-1)) > 0) {
    buf[n] = '\0';
    printf("%s", buf);
}
close(fd[0]);
wait(NULL);

La Prompt 2, AI-ul a generat:

void cleanup_dangling_symlinks(const char *prefix)
{
    DIR *dir = opendir(".");
    if(!dir) return;
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL)
    {
        if(strncmp(entry->d_name, prefix, strlen(prefix)) != 0)
            continue;
        struct stat st;
        struct stat lst;
        lstat(entry->d_name, &lst);
        if(!S_ISLNK(lst.st_mode))
            continue;
        if(stat(entry->d_name, &st) != 0) /* stat urmărește symlink-ul */
        {
            printf("Dangling symlink detectat: %s — stergere.\n", entry->d_name);
            unlink(entry->d_name);
        }
    }
    closedir(dir);
}

5. Probleme identificate

Pentru Prompt 1: 

Codul generat era corect ca structură de bază, dar nu acoperea cazul cu două fork-uri: hub_mon care fork-uiește monitorul. 
Am trebuit să adaptez pentru arhitectura cu trei procese: city_hub → hub_mon → monitor_reports, și să mă asigur că fiecare proces închide capetele de pipe pe care nu le folosește.
Un alt aspect pe care AI-ul nu l-a menționat explicit: în hub_mon, după ce monitor-ul face exec, trebuie să închidem filedes[1] și în hub_mon (nu doar în copil), altfel pipe-ul nu se închide niciodată și read() din hub_mon blochează la infinit chiar dacă monitorul s-a terminat.

Pentru Prompt 2: 

Funcția generată nu verifica că lstat() a reușit înainte de a folosi rezultatul. De asemenea, nu trata cazul în care opendir() eșuează cu un mesaj de eroare explicit. Funcția nu includea nici header-ul necesar (<dirent.h>).

6. Îmbunătățiri aduse

Pentru pipe + dup2: Am adaptat structura generată pentru arhitectura cu trei niveluri, adăugând închiderea corectă a tuturor descriptorilor în fiecare proces:

close(filedes[1]);
Pentru dangling symlinks: Am adăugat verificarea returnului lstat() și mesaje de eroare mai clare:
cvoid cleanup_dangling_symlinks(const char *prefix)
{
    DIR *dir = opendir(".");
    if(!dir)
    {
        perror("Nu pot deschide directorul curent");
        return;
    }
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL)
    {
        if(strncmp(entry->d_name, prefix, strlen(prefix)) != 0)
            continue;
        struct stat lst;
        if(lstat(entry->d_name, &lst) < 0)
            continue;
        if(!S_ISLNK(lst.st_mode))
            continue;
        struct stat st;
        if(stat(entry->d_name, &st) != 0)
        {
            printf("Avertisment: symlink dangling detectat: %s. Se sterge.\n", entry->d_name);
            unlink(entry->d_name);
        }
    }
    closedir(dir);
}
7. Ce am învățat

Cum funcționează pipe-ul la nivel de descriptori de fișiere și de ce e important să închidem capetele nefolosite
Că dup2(fd, STDOUT_FILENO) face ca printf să scrie în pipe, nu pe terminal
Diferența dintre stat() și lstat(): lstat() nu urmărește symlink-ul, stat() îl urmărește.
Că AI-ul generează adesea exemple simple care nu acoperă cazurile de eroare, și că aceste cazuri sunt tocmai cele mai importante de testat

8. Concluzie

Pentru Phase 3, AI-ul a fost util mai ales pentru a înțelege conceptele noi. 
