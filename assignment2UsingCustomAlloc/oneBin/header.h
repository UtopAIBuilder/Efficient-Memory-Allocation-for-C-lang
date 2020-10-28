
struct node
{
	unsigned int val;
	struct node * next;
};
struct polyNode
{
	int deg;
	float coef;
	struct polyNode* next,*prev;
};
struct node* addition(struct node *,struct node *);
struct node* mul(struct node *,struct node *);
int printList(struct node*);
struct node* expressionEval();
int printPoly(struct polyNode*);
void polyDiv(struct polyNode*,struct polyNode*);
void menu();
struct node* inputTry();
void deletePolylist(struct polyNode*);
struct polyNode* inputPoly();