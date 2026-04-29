AI Usage Report

1. Instrument utilizat

Pentru partea asistată de inteligență artificială am folosit ChatGPT (OpenAI)**, ca suport pentru generarea și analizarea unor funcții cerute în enunț.

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
