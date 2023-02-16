#include <gradient_descent.h>

double linmin(double point[], double h[], int n, double (*func)(double[]));
double brent(double ax, double bx, double cx, double (*func)(double x, struct f1DimParam), double tol, double* xmin, struct f1DimParam param);
void mnbrak(double* ax, double* bx, double* cx, double* fa, double* fb, double* fc, double (*func)(double x, struct f1DimParam), struct f1DimParam param);
double f1dim(double x, struct f1DimParam param);

// TODO - Calculate finite difference with 2 orders and let user specify it
// TODO - Give option of 1st order or second order (if not in loop)

double gradient_descent(double (*func)(double[]), void (*dfunc)(double[], double[]), double point[], int n) { // TODO - point = initialGuess
    // Init vectors
    double *g = malloc(n * sizeof(double));
    double *h = malloc(n * sizeof(double));
    double *xi = malloc(n * sizeof(double));
    double ret, fp, dgg, gg, gam;

    // Initialise g and h to the inverse of the gradient
    fp = (*func)(point);
    (*dfunc)(point, xi);
    for (int i = 0; i < n; i++) {
        g[i] = -xi[i];
        xi[i] = h[i] = g[i]; // TODO - consider memcpy
    }

    for (int its = 0; its <= ITMAX; its++) { // TODO - ITMAX parameter
        // Move point to the minimum along the h direction
        ret = linmin(point, xi, n, func); // TODO - maybe in the future improve with dlinmin if necessarygit

        // Checks if we are not moving enough (i.e we reached a min)
        // Normal function exit
        if (2.0*fabs(ret-fp) <= TOL*(fabs(ret)+fabs(fp)+EPS)) { // TODO - Tolerance defined by user EPS
            return ret;
        }

        fp = ret;
        (*dfunc)(point,xi);
        dgg=gg=0.0; // TODO - Refer to paper for mathematical names
        for (int j=0;j<n;j++) {// TODO - Make vector multiply in util.h if useful
            gg += g[j]*g[j];
            dgg += (xi[j]+g[j])*xi[j];
        }

        // Gradient is 0 we are done (maybe introduce a tolerance check here)
        if (gg == 0.0) {
            return ret;
        }

        gam=dgg/gg; // TODO - divide  by gg + Small epsilon and allows to remove if
        for (int j=0;j<n;j++) {
            g[j] = -xi[j];
            xi[j]=h[j]=g[j]+gam*h[j];
        }
    }
    
    // We should not reach here something went wrong
    return 0.0;
}

double linmin(double point[], double h[], int n, double (*func)(double[])) {
    int j;
    double xx,xmin,fx,fb,fa,bx,ax,ret;

    struct f1DimParam param;
    param.ncom = n;
    param.xicom = malloc(n * sizeof(double));
    param.pcom = malloc(n * sizeof(double));
    param.nrfunc = func;
    for (j=0;j<=n;j++) {
        param.pcom[j] = point[j];
        param.xicom[j]= h[j];
    }

    ax=0.0;
    xx=1.0;
    mnbrak(&ax,&xx,&bx,&fa,&fx,&fb,f1dim, param);
    ret = brent(ax,xx,bx,f1dim,TOL,&xmin, param);

    for (j=0;j<n;j++) { 
        h[j] *= xmin;
        point[j] += h[j];
    }

    free(param.xicom);
    free(param.pcom);

    return ret;
}

double brent(double ax, double bx, double cx, double (*func)(double x, struct f1DimParam), double tol, double* xmin, struct f1DimParam param) {
    int iter;
    double a,b,d,etemp,fu,fv,fw,fx,p,q,r,tol1,tol2,u,v,w,x,xm;
    double e=0.0;

    a=(ax < cx ? ax : cx);
    b=(ax > cx ? ax : cx);
    x=w=v=bx;
    fw=fv=fx=(*func)(x, param);

    for (iter=1;iter<=ITMAX;iter++) {
        xm=0.5*(a+b);
        tol2=2.0*(tol1=tol*fabs(x)+ZEPS);

        if (fabs(x-xm) <= (tol2-0.5*(b-a))) {
            *xmin=x;
            return fx;
        }

        if (fabs(e) > tol1) {
            r=(x-w)*(fx-fv);
            q=(x-v)*(fx-fw);
            p=(x-v)*q-(x-w)*r;
            q=2.0*(q-r);
            if (q > 0.0) p = -p;
            q=fabs(q);
            etemp=e;
            e=d;
            if (fabs(p) >= fabs(0.5*q*etemp) || p <= q*(a-x) || p >= q*(b-x)) {
                d=CGOLD*(e=(x >= xm ? a-x : b-x));
            } 
            else {
                d=p/q;
                u=x+d;
                if (u-a < tol2 || b-u < tol2)
                    d=SIGN(tol1,xm-x);
            }
        }
        else {
            d=CGOLD*(e=(x >= xm ? a-x : b-x));
        }

        u=(fabs(d) >= tol1 ? x+d : x+SIGN(tol1,d));
        fu=(*func)(u, param);

        if (fu <= fx) {
            if (u >= x) a=x; else b=x;
            SHFT(v,w,x,u);
            SHFT(fv,fw,fx,fu);
        }
        else {
            if (u < x) a=u; else b=u;
            if (fu <= fw || w == x) {
                v=w;
                w=u;
                fv=fw;
                fw=fu;
            } else if (fu <= fv || v == x || v == w) {
                v=u;
                fv=fu;
            }
        }
    }
    // We should not reach this point to many iterations
    *xmin=x;
    return fx;
}

void mnbrak(double* ax, double* bx, double* cx, double* fa, double* fb, double* fc, double (*func)(double x, struct f1DimParam), struct f1DimParam param) {
    double ulim,u,r,q,fu,dum;

    *fa=(*func)(*ax, param);
    *fb=(*func)(*bx, param);

    if (*fb > *fa) {
        SHFT(dum,*ax,*bx,dum);
        SHFT(dum,*fb,*fa,dum);
    }

    *cx=(*bx)+GOLD*(*bx-*ax);
    *fc=(*func)(*cx, param);

    while (*fb > *fc) {
        r=(*bx-*ax)*(*fb-*fc);
        q=(*bx-*cx)*(*fb-*fa);
        u=(*bx)-((*bx-*cx)*q-(*bx-*ax)*r)/(2.0*SIGN(FMAX(fabs(q-r),TINY),q-r));

        ulim=(*bx)+GLIMIT*(*cx-*bx);
        if ((*bx-u)*(u-*cx) > 0.0) {
            fu=(*func)(u, param);
            if (fu < *fc) {
                *ax=(*bx);
                *bx=u;
                *fa=(*fb);
                *fb=fu;
                return;
            }
            else if (fu > *fb) {
                *cx=u;
                *fc=fu;
                return;
            }
            u=(*cx)+GOLD*(*cx-*bx);
            fu=(*func)(u, param); 
        }
        else if ((*cx-u)*(u-ulim) > 0.0) {
            fu=(*func)(u, param);
            if (fu < *fc) {
                SHFT(*bx,*cx,u,*cx+GOLD*(*cx-*bx));
                SHFT(*fb,*fc,fu,(*func)(u, param));
            }
        }
        else if ((u-ulim)*(ulim-*cx) >= 0.0) {
            u=ulim;
            fu=(*func)(u, param);
        }
        else {
            u=(*cx)+GOLD*(*cx-*bx);
            fu=(*func)(u, param);
        }
        SHFT(*ax,*bx,*cx,u);
        SHFT(*fa,*fb,*fc,fu);
    }
}

double f1dim(double x, struct f1DimParam param) {
    int j;
    double f,*xt;
    xt=malloc(param.ncom * sizeof(double));
    for (j=0;j<param.ncom;j++) xt[j]=param.pcom[j]+x*param.xicom[j];
    f=(*(param.nrfunc))(xt);
    free(xt);
    return f;
}