#include <bits/stdc++.h>
using namespace std;

// We model terms: a * x^b * sin^c(x) * cos^d(x)
struct Term {
    long long a; int b, c, d;
};

struct Poly {
    vector<Term> t;
    Poly() {}
    explicit Poly(const vector<Term>& v): t(v) {}

    static bool cmpKey(const Term& x, const Term& y){
        // order by b desc, then c desc, then d desc (or any consistent order)
        if(x.b!=y.b) return x.b>y.b;
        if(x.c!=y.c) return x.c>y.c;
        return x.d>y.d;
    }

    void simplify(){
        // remove zero terms, sort, merge by exponents
        vector<Term> v;
        v.reserve(t.size());
        for(auto &e: t){ if(e.a!=0) v.push_back(e); }
        sort(v.begin(), v.end(), [](const Term& x, const Term& y){
            if(x.b!=y.b) return x.b>y.b;
            if(x.c!=y.c) return x.c>y.c;
            return x.d>y.d;
        });
        vector<Term> r;
        for(auto &e: v){
            if(r.empty() || r.back().b!=e.b || r.back().c!=e.c || r.back().d!=e.d){
                r.push_back(e);
            }else{
                r.back().a += e.a;
                if(r.back().a==0) r.pop_back();
            }
        }
        t.swap(r);
    }

    Poly operator+(const Poly& o) const{
        Poly r; r.t.reserve(t.size()+o.t.size());
        r.t.insert(r.t.end(), t.begin(), t.end());
        r.t.insert(r.t.end(), o.t.begin(), o.t.end());
        r.simplify();
        return r;
    }
    Poly operator-(const Poly& o) const{
        Poly r; r.t.reserve(t.size()+o.t.size());
        r.t.insert(r.t.end(), t.begin(), t.end());
        for(auto &e: o.t){ r.t.push_back(Term{-e.a, e.b, e.c, e.d}); }
        r.simplify();
        return r;
    }
    Poly operator*(const Poly& o) const{
        Poly r; r.t.reserve(t.size()*o.t.size());
        for(const auto &x: t){
            for(const auto &y: o.t){
                long long na = x.a * y.a;
                int nb = x.b + y.b;
                int nc = x.c + y.c;
                int nd = x.d + y.d;
                r.t.push_back(Term{na, nb, nc, nd});
            }
        }
        r.simplify();
        return r;
    }
    Poly derivate() const{
        Poly res;
        // derivative of a*x^b*sin^c*cos^d yields up to 3 terms
        for(const auto &x: t){
            // d/dx x-part
            if(x.b>0){
                res.t.push_back(Term{ x.a * x.b, x.b-1, x.c, x.d });
            }
            // d/dx sin^c -> c*sin^{c-1}*cos
            if(x.c>0){
                res.t.push_back(Term{ x.a * x.c, x.b, x.c-1, x.d+1 });
            }
            // d/dx cos^d -> -d*sin*cos^{d-1}
            if(x.d>0){
                res.t.push_back(Term{ -x.a * x.d, x.b, x.c+1, x.d-1 });
            }
        }
        res.simplify();
        return res;
    }
};

struct Frac {
    Poly p, q; // p/q
    Frac(){}
    explicit Frac(long long c){ p.t.push_back(Term{c,0,0,0}); q.t.push_back(Term{1,0,0,0}); }
    Frac(const Poly& _p, const Poly& _q){ p=_p; q=_q; }

    Frac operator+(const Frac& o) const{ // (p1/q1 + p2/q2) = (p1*q2 + p2*q1)/(q1*q2)
        Poly np = (p * o.q) + (o.p * q);
        Poly nq = (q * o.q);
        return Frac(np, nq);
    }
    Frac operator-(const Frac& o) const{ // (p1*q2 - p2*q1)/(q1*q2)
        Poly np = (p * o.q) - (o.p * q);
        Poly nq = (q * o.q);
        return Frac(np, nq);
    }
    Frac operator*(const Frac& o) const{ // (p1*p2)/(q1*q2)
        return Frac(p * o.p, q * o.q);
    }
    Frac operator/(const Frac& o) const{ // (p1*q2)/(q1*p2)
        return Frac(p * o.q, q * o.p);
    }
    Frac derivate() const{ // (p' * q - q' * p) / (q * q)
        Poly np = (p.derivate() * q) - (q.derivate() * p);
        Poly nq = (q * q);
        return Frac(np, nq);
    }
};

static bool isDigit(char c){ return c>='0' && c<='9'; }

// Parsing utilities
// get_number from substring s[l..r) just before a symbol. If none digits, returns +/-1 depending on leading sign
long long get_number(const string& s, int l, int r){
    bool neg=false; long long val=0; bool hasDigit=false; int i=l;
    if(i<r && (s[i]=='+' || s[i]=='-')){ neg = (s[i]=='-'); i++; }
    while(i<r && isDigit(s[i])){ hasDigit=true; val = val*10 + (s[i]-'0'); i++; }
    if(!hasDigit) return neg? -1: 1; // implicit 1
    return neg? -val: val;
}

// Parse a single term from s[l..r)
Term get_term(const string& s, int l, int r){
    // structure like a x^b sin^c x cos^d x, but compact: e.g., -x^2sinxcos^3x or -2xsin^2x
    long long a = get_number(s,l,r);
    int b=0,c=0,d=0;
    // scan through looking for x (not part of sin/cos), sin, cos blocks
    for(int i=l;i<r;){
        if(s[i]=='+'){ i++; continue; }
        if(s[i]=='-'){ i++; continue; }
        if(s[i]=='x'){
            i++;
            int exp=1;
            if(i<r && s[i]=='^'){
                i++;
                long long v=0; while(i<r && isDigit(s[i])){ v=v*10+(s[i]-'0'); i++; }
                exp=(int)v;
            }
            b += exp; // multiplicative so add exponents
            continue;
        }
        if(i+2<r && s[i]=='s' && s[i+1]=='i' && s[i+2]=='n'){
            i+=3; // after 'sin'
            int exp=1;
            if(i<r && s[i]=='^'){
                i++;
                long long v=0; while(i<r && isDigit(s[i])){ v=v*10+(s[i]-'0'); i++; }
                exp=(int)v;
            }
            if(i<r && s[i]=='x') i++; // consume x
            c += exp;
            continue;
        }
        if(i+2<r && s[i]=='c' && s[i+1]=='o' && s[i+2]=='s'){
            i+=3;
            int exp=1;
            if(i<r && s[i]=='^'){
                i++;
                long long v=0; while(i<r && isDigit(s[i])){ v=v*10+(s[i]-'0'); i++; }
                exp=(int)v;
            }
            if(i<r && s[i]=='x') i++;
            d += exp;
            continue;
        }
        // skip other characters (like parentheses won't appear in term parsing region)
        i++;
    }
    return Term{a,(int)b,(int)c,(int)d};
}

// expression parser: handle +,-,*,/, parentheses producing Frac using defined arithmetic
Frac dfs(const string& s, int l, int r);

int find_main_plus_minus(const string& s, int l, int r){
    int depth=0; int pos=-1; // rightmost lowest precedence + or -
    for(int i=l;i<r;i++){
        char c=s[i];
        if(c=='(') depth++;
        else if(c==')') depth--;
        else if(depth==0 && (c=='+'||c=='-')){
            // treat leading sign (at l) as part of number, not operator
            if(i==l) continue;
            pos=i; // choose rightmost to implement left-to-right grouping
        }
    }
    return pos;
}

int find_main_mul_div(const string& s, int l, int r){
    int depth=0; int pos=-1;
    for(int i=l;i<r;i++){
        char c=s[i];
        if(c=='(') depth++;
        else if(c==')') depth--;
        else if(depth==0 && (c=='*' || c=='/')) pos=i;
    }
    return pos;
}

// trim outer parentheses
bool has_outer_paren(const string& s, int l, int r){
    if(l>=r || s[l]!='(' || s[r-1]!=')') return false;
    int depth=0;
    for(int i=l;i<r;i++){
        if(s[i]=='(') depth++;
        else if(s[i]==')'){
            depth--;
            if(depth==0 && i!=r-1) return false;
        }
    }
    return true;
}

Frac atom(const string& s, int l, int r){
    // either parenthesized expression or a sum of terms without top-level ops
    // Here if substring contains '+' or '-' at top-level (beyond leading sign), it would have been split earlier.
    // So it is either a product of compacted tokens -> treat as a single term
    Term t = get_term(s,l,r);
    Poly p; p.t.push_back(t);
    Poly q; q.t.push_back(Term{1,0,0,0});
    return Frac(p,q);
}

Frac dfs(const string& s, int l, int r){
    while(l<r && s[l]==' ') l++;
    while(r>l && s[r-1]==' ') r--;
    if(l>=r) return Frac(0);
    while(has_outer_paren(s,l,r)){
        l++; r--; while(l<r && s[l]==' ') l++; while(r>l && s[r-1]==' ') r--;
    }
    // Leading '+' or '-' are treated within term parsing; do not split here
    int k = find_main_plus_minus(s,l,r);
    if(k!=-1){
        Frac L = dfs(s,l,k);
        Frac R = dfs(s,k+1,r);
        if(s[k]=='+') return L + R;
        else return L - R;
    }
    k = find_main_mul_div(s,l,r);
    if(k!=-1){
        Frac L = dfs(s,l,k);
        Frac R = dfs(s,k+1,r);
        if(s[k]=='*') return L * R;
        else return L / R;
    }
    if(s[l]=='(' && s[r-1]==')'){
        return dfs(s,l+1,r-1);
    }
    return atom(s,l,r);
}

// Output formatting of Poly and Frac per problem rules
string term_to_str(const Term& t){
    // format: ax^b sin^c x cos^d x compacted like: a x^b sin^c x cos^d x
    // Output rules resemble samples: combine factors in order x^b, sin^c x, sin x as sinx, cos similarly.
    long long a=t.a; int b=t.b, c=t.c, d=t.d;
    if(a==0) return string();
    string res;
    // coefficient: handle sign and magnitude; if magnitude is 1 and there are variable parts, omit '1'
    bool hasVar = (b||c||d);
    long long mag = llabs(a);
    if(a<0) res.push_back('-');
    if(!hasVar || mag!=1){ res += to_string(mag); }
    if(b){ res += (hasVar||mag!=1?"":""); res += "x"; if(b!=1){ res += "^"+to_string(b);} }
    if(c){ res += string("sin"); if(c!=1){ res += "^"+to_string(c);} res += "x"; }
    if(d){ res += string("cos"); if(d!=1){ res += "^"+to_string(d);} res += "x"; }
    if(res.empty()) res = "0";
    return res;
}

string poly_to_str(const Poly& p){
    if(p.t.empty()) return "0";
    string out;
    for(size_t i=0;i<p.t.size();++i){
        Term t=p.t[i];
        if(t.a==0) continue;
        string ts = term_to_str(t);
        if(ts.empty()) continue;
        if(i==0){
            out += ts;
        }else{
            if(ts[0]=='-'){
                out += ts; // already has '-'
            }else{
                out += "+"; out += ts;
            }
        }
    }
    if(out.empty()) return "0";
    return out;
}

static bool is_one_poly(const Poly& q){
    return q.t.size()==1 && q.t[0].a==1 && q.t[0].b==0 && q.t[0].c==0 && q.t[0].d==0;
}

string frac_to_str(const Frac& f){
    string P = poly_to_str(f.p);
    string Q = poly_to_str(f.q);
    if(is_one_poly(f.q)){
        return P;
    }
    // Always output p/q; samples show if denominator 1, output just numerator? Actually samples show simplified forms like (x-sinx+1)/(x-sinx)
    // They do not add parentheses for simple polynomials without '+'? For safety, wrap p and q in parentheses only if they contain '+' or '-' excluding leading sign.
    auto needParen=[&](const string& s){
        for(size_t i=1;i<s.size();++i){ if(s[i]=='+'||s[i]=='-') return true; }
        return false;
    };
    bool pParen = needParen(P);
    bool qParen = needParen(Q);
    string out;
    if(pParen) out += "("+P+")"; else out += P;
    out += "/";
    if(qParen) out += "("+Q+")"; else out += Q;
    return out;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s; if(!getline(cin,s)) return 0; 
    int n = (int)s.size();
    Frac f = dfs(s,0,n);
    Frac g = f.derivate();
    cout<<frac_to_str(f)<<"\n"<<frac_to_str(g)<<"\n";
    return 0;
}
