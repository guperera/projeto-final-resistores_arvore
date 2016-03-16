#include <stdio.h>
#include <stdlib.h>

struct resistor{
    float valor;
    int serie;
    float erro;
    float potencia;
    int quantidade;
};

struct lista{
    struct resistor *info;
    struct lista *ant;
    struct lista *prox;
};

struct cabeca{
    int total;
    struct lista *inicio;
    struct lista *fim;
};

struct no_arvore
{
    int freq;
    int qtd;
    struct lista *inicio_li;
    struct no_arvore *dir;
    struct no_arvore *esq;
};

struct lista_no
{
    struct no_arvore *end_no;
    struct lista_no *prox;
};

void inic_cab(struct cabeca*);
void le_arquivo(struct cabeca*);
void imprime_lista(struct cabeca*);
void adiciona_item(struct cabeca*, struct resistor*);
void libera(struct cabeca*);
struct lista* busca(struct cabeca*, struct resistor*);
int menu();
void menu_adiciona(struct cabeca*);
void menu_busca(struct cabeca*);
void menu_apagar(struct cabeca*);
void menu_sair(struct cabeca*);
void ordena(struct cabeca*);

void menu_cria_arvore(struct cabeca*);
struct no_arvore *inic_no();
struct no_arvore *cria_no(struct lista*, int, int, struct no_arvore *, struct no_arvore *);
void inic_lista(struct lista_no *);
void adiciona_lista(struct lista_no*, struct no_arvore*);
void imprime_lista_arvore(struct lista_no*);
void ordena_lista_arvore(struct lista_no*);
struct no_arvore *cria_arvore(struct lista_no *);
void remove_primeiro(struct lista_no *);
void imprime_arvore(struct no_arvore*);
void imprime_serie(struct no_arvore*, int, int*);

int main()
{
    struct cabeca cab;
    int sair = 1;
    inic_cab(&cab);
    le_arquivo(&cab);
    while(sair)
    {
        switch(menu())
        {
        case 1:
            imprime_lista(&cab);
            break;
        case 2:
            menu_adiciona(&cab);
            break;
        case 3:
            menu_busca(&cab);
            break;
        case 4:
            menu_apagar(&cab);
            break;
        case 5:
            menu_cria_arvore(&cab);
            break;
        case 6:
            menu_sair(&cab);
            sair = 0;
            break;
        default:
            break;
        }
        if(sair)
        {
            printf("\n\t");
            system("pause");
        }
    }
    libera(&cab);
    return 0;
}

void inic_cab(struct cabeca *c)
{
   c->total = 0;
   c->inicio = NULL;
   c->fim = NULL;
}

void le_arquivo(struct cabeca* cab)
{
    FILE *fp;
    int n = 0;
    struct resistor res;
    fp = fopen("tabela.txt", "r");
    if (fp == NULL)
        printf("Erro ao abrir arquivo");
    while(1)
    {
        n = fscanf(fp,"%f %d %f %f %d", &res.valor, &res.serie, &res.erro, &res.potencia, &res.quantidade);
        if(n == EOF)
            break;
        adiciona_item(cab, &res);
    }
    fclose(fp);
    ordena(cab);
}

void imprime_lista(struct cabeca* cab)
{
    char *str;
    struct lista *li;
    li = cab->inicio;
    printf("\n\tLista competa (%d Elementos):\n", cab->total);
    while(li != NULL)
    {
        printf("\tValor: %.2f", li->info->valor);
        switch(li->info->serie)
                {
                case 1:
                    str = "E6";
                    break;
                case 2:
                    str = "E12";
                    break;
                case 3:
                    str = "E24";
                    break;
                case 4:
                    str = "E48";
                    break;
                case 5:
                    str = "E96";
                    break;
                case 6:
                    str = "E192";
                    break;
                default:
                    break;
                }
        printf(" Serie: %s", str);
        printf(" Erro: %.2f%% Potencia: %.2fW Quantidade: %d\n", li->info->erro, li->info->potencia, li->info->quantidade);
        li = li->prox;
    }
}

void adiciona_item(struct cabeca* cab, struct resistor* res2)
{
    char confirma;
    struct resistor *res = (struct resistor *)malloc(sizeof(struct resistor));
    struct lista *li2 = (struct lista *)malloc(sizeof(struct lista));
    struct lista *li;
    li = busca(cab, res2);
    if(li != NULL)
    {
        printf("\n\tResistor existente, adicionar a quantidade (S - SIM ou N - NAO): ");
        fflush(stdin);
        scanf("%c", &confirma);
        if(confirma == 'S' || confirma == 's')
            li->info->quantidade += res2->quantidade;
        else
            printf("\n\t-> Resistor nao adicionado!!!");
    }
    else
    {
        res->erro = res2->erro;
        res->potencia = res2->potencia;
        res->quantidade = res2->quantidade;
        res->serie = res2->serie;
        res->valor = res2->valor;
        li = cab->fim;
        if(li == NULL)
        {
            cab->inicio = li2;
            cab->fim = li2;
            cab->total = 1;
            li2->info = res;
            li2->ant = NULL;
            li2->prox = NULL;
        }
        else
        {
            li->prox = li2;
            cab->total += 1;
            cab->fim = li2;
            li2->ant = li;
            li2->prox = NULL;
            li2->info = res;
        }
    }
}

void libera(struct cabeca* cab)
{
    struct lista *li;
    struct lista *li2;
    li = cab->inicio;
    while(li != NULL)
    {
        li2 = li->prox;
        free(li->info);
        free(li);
        li = li2;
    }
    inic_cab(cab);
}

struct lista* busca(struct cabeca *cab, struct resistor* res)
{
    struct lista *li;
    li = cab->inicio;
    while(li != NULL)
    {
        if(li->info->serie == res->serie && li->info->potencia == res->potencia && li->info->valor == res->valor)
                return li;
        li = li->prox;
    }
    return NULL;
}

int menu()
{
    int opcao;
    system("cls");
    printf("\t***************************************\n");
    printf("\t* CONTROLE DO ESTOQUE DO ALMOXARIFADO *\n");
    printf("\t*             RESISTORES              *\n");
    printf("\t***************************************\n\n");
    printf("\t1) Imprimir lista\n");
    printf("\t2) Adicionar resistor\n");
    printf("\t3) Buscar resistor\n");
    printf("\t4) Apagar resistor\n");
    printf("\t5) Cria arvore\n");
    printf("\t6) Sair\n\n");
    printf("\tOpcao: ");
    fflush(stdin);
    scanf("%d", &opcao);
    return opcao;
}

void menu_adiciona(struct cabeca* cab)
{
    struct resistor res;
    char confirma;
    printf("\n\tEntre com os dados do resistor:\n");
    printf("\tValor (Ohms): ");
    fflush(stdin);
    scanf("%f", &res.valor);
    printf("\tSerie:\n\t  1) E6\n\t  2) E12\n\t  3) E24\n\t  4) E48\n\t  5) E96\n\t  6) E192\n\t  Valor: ");
    fflush(stdin);
    scanf("%d", &res.serie);
    if(res.serie == 6)
    {
        printf("\tPara serie E192 informe o Erro (0.5%% ou 0.25%% ou 0.1%%): ");
        fflush(stdin);
        scanf("%f", &res.erro);
    }
    printf("\tPotencia (W): ");
    fflush(stdin);
    scanf("%f", &res.potencia);
    printf("\tQuantidade: ");
    fflush(stdin);
    scanf("%d", &res.quantidade);
    printf("\tConfirma a inclusao (S - SIM ou N - NAO): ");
    fflush(stdin);
    scanf("%c", &confirma);
    if(confirma == 'S' || confirma == 's')
    {
        if((res.serie >= 1) && (res.serie <= 6))
        {


            if(res.serie == 6 && !(res.erro == 0.5 || res.erro == 0.25 || res.erro == 0.1))
                printf("\n\t-> Erro (%%) inexistente - Resistor nao adicionado!!!");
            else
            {
                switch(res.serie)
                {
                case 1:
                    res.erro = 20;
                    break;
                case 2:
                    res.erro = 10;
                    break;
                case 3:
                    res.erro = 5;
                    break;
                case 4:
                    res.erro = 4;
                    break;
                case 5:
                    res.erro = 2;
                    break;
                default:
                    break;
                }
                adiciona_item(cab, &res);
                ordena(cab);
                if((busca(cab, &res)))
                    printf("\n\t-> Resistor adicionado com sucesso!!!");
            }
        }
        else
            printf("\n\t-> Serie inexistente - Resistor nao adicionado!!!");
    }
    else
        printf("\n\t-> Resistor nao adicionado!!!");
}

void menu_busca(struct cabeca* cab)
{
    struct lista *li;
    li = cab->inicio;
    int flag, serie, quantidade;
    float valor,potencia;
    char *str;
    flag = 0;
    serie = quantidade = 0;
    valor = potencia = 0;
    char str_s[12], str_q[12], str_v[12], str_p[12];
    printf("\n\tEntre com os dados do resistor:\n");
    printf("\tValor (Ohms): ");
    fflush(stdin);
    fgets(str_v, sizeof(str_v), stdin);
    printf("\tSerie:\n\t  1) E6\n\t  2) E12\n\t  3) E24\n\t  4) E48\n\t  5) E96\n\t  6) E192\n\t  Valor: ");
    fflush(stdin);
    fgets(str_s, sizeof(str_s), stdin);
    printf("\tPotencia (W): ");
    fflush(stdin);
    fgets(str_p, sizeof(str_p), stdin);
    printf("\tQuantidade: ");
    fflush(stdin);
    fgets(str_q, sizeof(str_q), stdin);
    valor = atof(str_v);
    serie = atoi(str_s);
    potencia = atof(str_p);
    quantidade = atoi(str_q);
    while(li != NULL)
    {
        if(valor == li->info->valor || valor == 0)
            if(serie == li->info->serie || serie == 0)
                if(potencia == li->info->potencia || potencia == 0)
                    if(quantidade == li->info->quantidade || quantidade == 0)
                    {
                        printf("\tValor: %.2f", li->info->valor);
                        switch(li->info->serie)
                        {
                        case 1:
                            str = "E6";
                            break;
                        case 2:
                            str = "E12";
                            break;
                        case 3:
                            str = "E24";
                            break;
                        case 4:
                            str = "E48";
                            break;
                        case 5:
                            str = "E96";
                            break;
                        case 6:
                            str = "E192";
                            break;
                        default:
                            break;
                        }
                        printf(" Serie: %s", str);
                        printf(" Erro: %.2f%% Potencia: %.2fW Quantidade: %d\n", li->info->erro, li->info->potencia, li->info->quantidade);
                        flag = 1;
                    }
        li = li->prox;
    }
    if(flag == 0)
        printf("\n\t-> Resistor nao encontrado!!!");
}

void menu_apagar(struct cabeca* cab)
{
    int serie, flag;
    float valor, potencia;
    struct lista *li;
    li = cab->inicio;
    flag = 0;
    printf("\n\tEntre com os dados do resistor a ser apagado:\n");
    printf("\tValor (Ohms): ");
    fflush(stdin);
    scanf("%f", &valor);
    printf("\tSerie:\n\t  1) E6\n\t  2) E12\n\t  3) E24\n\t  4) E48\n\t  5) E96\n\t  6) E192\n\t  Valor: ");
    fflush(stdin);
    scanf("%d", &serie);
    printf("\tPotencia (W): ");
    fflush(stdin);
    scanf("%f", &potencia);
    while(li != NULL)
    {
        if(valor == li->info->valor && serie == li->info->serie && potencia == li->info->potencia)
        {
            if(li->ant == NULL)
            {
                cab->inicio = li->prox;
                li->prox->ant = NULL;
            }
            else
                li->ant->prox = li->prox;
            if(li->prox == NULL)
               cab->fim = li->ant;
            else
                li->prox->ant = li->ant;
            cab->total--;
            free(li);
            flag = 1;
        }
        li = li->prox;
    }
    if(flag == 1)
        printf("\n\t-> Resistor apagado com sucesso!!!");
    else
        printf("\n\t-> Resistor nao apagado!!!");
}

void menu_sair(struct cabeca* cab)
{
    FILE *fp;
    struct lista *li;
    li = cab->inicio;
    fp = fopen("tabela.txt", "w");
    if (fp == NULL)
        printf("Erro ao abrir arquivo");
    while(li != NULL)
    {
        fprintf(fp, "%.2f %d %.2f %.2f %d\n", li->info->valor, li->info->serie, li->info->erro, li->info->potencia, li->info->quantidade);
        li = li->prox;
    }
    fclose(fp);
}

void ordena(struct cabeca* cab)
{
    struct lista *li;
    struct resistor *res;
    int i, j, x, z;
    for(z = 0; z <= 2; z++)
    {
        for(i = cab->total; i > 1; i--)
            for(j = 0; j < i; j++)
            {
                li = cab->inicio;
                for(x = 1; x < j; x++)
                    li = li->prox;
                if((li->info->valor > li->prox->info->valor && li->info->serie == li->prox->info->serie && li->info->potencia == li->prox->info->potencia) || z == 0 || z==1)
                    if((li->info->potencia > li->prox->info->potencia && li->info->serie == li->prox->info->serie) || z == 0 || z == 2)
                        if((li->info->serie > li->prox->info->serie) || z == 1 || z == 2)
                        {
                            res = li->info;
                            li->info = li->prox->info;
                            li->prox->info = res;
                        }
            }
    }
}



struct no_arvore *inic_no()
{
    return NULL;
}

struct no_arvore *cria_no(struct lista *li, int f, int q, struct no_arvore *e, struct no_arvore *d)
{
    struct no_arvore *arv = (struct no_arvore*)malloc(sizeof(struct no_arvore));
    arv->inicio_li = li;
    arv->freq = f;
    arv->qtd = q;
    arv->dir = d;
    arv->esq = e;
    return arv;
}


void inic_lista(struct lista_no *li)
{
    li->end_no = NULL;
    li->prox = NULL;
}

void menu_cria_arvore(struct cabeca *cab)
{
    struct lista_no l_no;
    struct no_arvore *no_h;
    inic_lista(&l_no);
    struct lista *li, *inicio;
    int serie, qtd, opcao, flag;
    flag = 0;
    qtd = 1;
    li = cab->inicio;

    while(li != NULL)
    {
        serie = li->info->serie;
        inicio = li;
        while(li->prox != NULL)
        {
            if(serie != li->prox->info->serie)
                break;
            else
            {
                qtd++;
                li = li->prox;
            }
        }
        li = li->prox;
        adiciona_lista(&l_no, cria_no(inicio, serie, qtd, inic_no(), inic_no()));
        qtd = 1;
    }


    no_h = cria_arvore(&l_no);
    printf("\n\t->Arvore criada com sucesso!!!\n\n");
    printf("\t1) Imprimir arvore\n");
    printf("\t2) Buscar serie pela arvore\n");
    printf("\t3) voltar ao menu inicial\n\n");
    printf("\tOpcao: ");
    fflush(stdin);
    scanf("%d", &opcao);
    switch(opcao)
        {
        case 1:
            printf("\n");
            imprime_arvore(no_h);
            break;
        case 2:
            printf("\n\tEntre com o valor da serie: ");
            fflush(stdin);
            scanf("%d", &serie);
            imprime_serie(no_h, serie, &flag);
            if(!flag)
                printf("\n\t->Serie nao encontrada!!!");
            break;
        case 3:
            break;
        default:
            break;
        }
}

void adiciona_lista(struct lista_no *li, struct no_arvore *n)
{
    if(li->end_no == NULL)
    {
        li->end_no = n;
        li->prox = NULL;
    }
    else
    {
        struct lista_no *li2 = (struct lista_no*)malloc(sizeof(struct lista_no));
        li2->end_no = n;
        li2->prox = NULL;
        for(;li != NULL; li = li->prox)
        {
            if(li->prox == NULL)
            {
                li->prox = li2;
                break;
            }
        }
    }
}

void imprime_lista_arvore(struct lista_no *li)
{
    if(li != NULL)
    {
       while(li != NULL)
        {
            printf("\nSerie: %d, quantidade: %d, inicio: %p", li->end_no->freq, li->end_no->qtd, li->end_no->inicio_li);
            li = li->prox;
        }
    }
}

void ordena_lista_arvore(struct lista_no* li)
{
    struct lista_no *li2;
    struct lista_no *aux_l;
    struct lista_no *aux_l1;
    struct no_arvore *aux_n;
    aux_l = li;
    aux_l1 = li;
    while(aux_l != NULL)
    {
        while(li->prox != NULL)
        {
            li2 = li->prox;
            if(li->end_no->freq > li2->end_no->freq)
            {
                aux_n = li->end_no;
                li->end_no = li2->end_no;
                li2->end_no = aux_n;
            }
            else
                li = li2;
        }
        li = aux_l1;
        aux_l = aux_l->prox;
    }
}

struct no_arvore *cria_arvore(struct lista_no *li)
{
    struct no_arvore *Tr,*T1, *T2;
    while(li->prox != NULL)
    {
        T1 = li->end_no;
        T2 = li->prox->end_no;
        remove_primeiro(li);
        remove_primeiro(li);
        Tr = cria_no(NULL, (T1->freq + T2->freq), 0, T1, T2);
        //Tr = cria_no(i++, (T1->freq + T2->freq), T1, T2);
        adiciona_lista(li, Tr);
        ordena_lista_arvore(li);
        //printf("\n\n");
        //imprime_lista_arvore(li);
    }
    return Tr;
}

void remove_primeiro(struct lista_no *li)
{
    if(li->prox != NULL)
    {
        li->end_no = li->prox->end_no;
        li->prox = li->prox->prox;
    }
    else
        inic_lista(li);
}

void imprime_arvore(struct no_arvore *arv)
{
    if(arv != NULL)
    {
        printf("<P: %d, Q: %d", arv->freq, arv->qtd);
        imprime_arvore(arv->esq);
        printf(">");
        imprime_arvore(arv->dir);
        printf(">");
    }
    else
        printf("<");
}

void imprime_serie(struct no_arvore* arv, int serie, int *flag)
{
    if(arv != NULL)
    {
        if(arv->freq == serie && arv->qtd != 0)
        {
            int i;
            struct lista *li;
            char *str;
            li = arv->inicio_li;
            printf("\n");
            for(i=0; i<arv->qtd; i++)
            {
                printf("\tValor: %.2f", li->info->valor);
                switch(li->info->serie)
                {
                case 1:
                    str = "E6";
                    break;
                case 2:
                    str = "E12";
                    break;
                case 3:
                    str = "E24";
                    break;
                case 4:
                    str = "E48";
                    break;
                case 5:
                    str = "E96";
                    break;
                case 6:
                    str = "E192";
                    break;
                default:
                    break;
                }
                printf(" Serie: %s", str);
                printf(" Erro: %.2f%% Potencia: %.2fW Quantidade: %d\n", li->info->erro, li->info->potencia, li->info->quantidade);
                li = li->prox;
                *flag = 1;
            }
        }
        imprime_serie(arv->dir, serie, flag);
        imprime_serie(arv->esq, serie, flag);
    }
}
