#include "macros.h"
#include "math.h"
#include "sparse/spMatrix.h"
#include "defs.h"
#include "bjt.h"

void makeBjt(Bjt, numBjt, buf)
bjt *Bjt[];
int numBjt;
char *buf;
{
    bjt *inst;
    int j, nodeP, nodeN, atoi();
    char name[MAXFIELD], mname[MAXFIELD], node[MAXFIELD], num[MAXFIELD];
    double value, atof();

    j = 0;	
    j = getNextField(buf, name, j);
    j = getNextField(buf, node, j);
    nodeC = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeB = getMappedNode(node);
    j = getNextField(buf, node, j);
    nodeE = getMappedNode(node);
    j = getNextField(buf, mname, j);

    inst = CALLOC(bjt, 1);
    inst->name = (char *)strdup(name);
    inst->cNode = nodeC;
    inst->bNode = nodeB;
    inst->eNode = nodeE;
    inst->mname = (char *)strdup(mname);
    Bjt[numBjt] = inst;
}

void printBjt(Bjt, numBjt)
bjt *Bjt[];
int numBjt;
{
    int i;
    bjt *inst;

    for(i = 1; i <= numBjt; i++) {
	inst = Bjt[i];
	printf("%s\t%s\t%s\t%s\t%f\n", inst->name, NodeArray[inst->cNode], NodeArray[inst->bNode], NodeArray[inst->eNode], inst->mname);
    }
}




void setupBjt(Matrix, Rhs, Bjt, numBjt)
char *Matrix;
double *Rhs;
bjt *Bjt[];
int numBjt;
{
    int i, p,n;
    bjt *inst;

    /* do any preprocessing steps here */
    for(i = 1; i <= numBjt; i++) {
	inst = Bjt[i];
//	inst->branchNum += NumNodes;
	p = inst->pNode;
	n = inst->nNode;
	/* setup matrix and pointers */
	inst->ppp = spGetElement(Matrix, p, p);
	inst->ppn = spGetElement(Matrix, p, n);
	inst->pnp = spGetElement(Matrix, n, p);
	inst->pnn = spGetElement(Matrix, n, n);
        
////INITIAL CONDITION TO ACHIEVE CONVERGENCE
        Xk[p] += .35; 
        Xk[n] -= .35; 

    }
}

void loadBjt(Matrix, Rhs, Bjt, numBjt, Xk)
char *Matrix;
double *Rhs;
bjt *Bjt[];
int numBjt;
double* Xk;
{
  int i, p, n;
  bjt *inst;
  double gk, Ik,Id, Vd;
  double Is, Vt,area;

  for(i = 1; i <= numBjt; i++) {
    inst = Bjt[i];
    p = inst->pNode;
    n = inst->nNode;
///////////Getting Vd from Vp and Vn///////////////
    Vd = Xk[p]-Xk[n];
    area = inst->area;
////////Hard coding Is and Vt Values//////////////////
      Is = 100.0e-6;
      Vt = 0.7;
///////////////Id and gk Calculation///////////////////

     Id = area*Is*(exp(Vd/Vt)-1);
     gk = area*Is*exp(Vd/Vt)/Vt;
////////////////////Ik Calculation/////////////////////

     Ik = Id-gk*Vd;

    *(inst->ppp) += gk;
    *(inst->ppn) -= gk;
    *(inst->pnp) -= gk;
    *(inst->pnn) += gk;
    
    Rhs[p] -= Ik;
    Rhs[n] += Ik;
  }
}
