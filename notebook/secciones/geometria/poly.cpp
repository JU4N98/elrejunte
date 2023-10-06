struct poly{
	vector<pto> pt;
	
	poly(){}
	poly(vector<pto> pt_) : pt(pt_) {}
	
	void normalize(){ // delete collinear points first
		// this makes it clockwise
		if(pt[2].left(pt[0], pt[1])) reverse(pt.begin(), pt.end());
		int n=sz(pt), pi=0;
		forn(i, n)
			if(pt[i].x<pt[pi].x || (pt[i].x==pt[pi].x && pt[i].y<pt[pi].y))
				pi=i;
		rotate(pt.begin(), pt.begin()+pi, pt.end());
	}
		
	bool is_convex(){ // delete collinear points first
		int N = sz(pt);
		if(N<3) return false;
		bool isLeft=pt[0].left(pt[1], pt[2]);
		forr(i, 1, sz(pt))
			if(pt[i].left(pt[(i+1)%N], pt[(i+2)%N]) != isLeft)
				return false;
		return true; 
	}
	
	// for convex or concave polygons
	// excludes boundaries, check it manually
	bool inside(pto p) { // O(n)  
		bool c = false;
		forn(i, sz(pt)){
			int j=(i+1)%sz(pt);
			if((pt[j].y>p.y) != (pt[i].y > p.y) && 
			(p.x < (pt[i].x - pt[j].x) * (p.y-pt[j].y) / (pt[i].y - pt[j].y) + pt[j].x))
				c = !c;
		}
		return c;
	}
	
	bool inside_convex(pto p){ // O(lg(n)) normalize first
		if(p.left(pt[0], pt[1]) || p.left(pt[sz(pt)-1], pt[0])) return false;
		int a=1, b=sz(pt)-1;
		while(b-a>1){
			int c=(a+b)/2;
			if(!p.left(pt[0], pt[c])) a=c;
			else b=c;
		}
		return !p.left(pt[a], pt[a+1]);
	}
	
	// cuts this along line ab and return the left side 
	// (swap a, b for the right one)
	poly cut(pto a, pto b){ // O(n)
		vector<pto> ret;
		forn(i, sz(pt)){
			ld left1=(b-a)^(pt[i]-a), left2=(b-a)^(pt[(i+1)%sz(pt)]-a);
			if(left1>=0) ret.pb(pt[i]);
			if(left1*left2<0)
				ret.pb(line(pt[i], pt[(i+1)%sz(pt)]).inter(line(a, b)));
		}
		return poly(ret);
	}

	// addition of polygons
	poly minkowski(poly p) { // O(n+m) n=|this|,m=|p| 
		this->normalize(); p.normalize();
		vector<pto> a = (*this).pt, b = p.pt;
		a.pb(a[0]); a.pb(a[1]);
		b.pb(b[0]); b.pb(b[1]);
		vector<pto> sum;
		int i = 0, j = 0;
		while(i<sz(a)-2 || j<sz(b)-2) {
			sum.pb(a[i]+b[j]);
			T cross = (a[i+1]-a[i])^(b[j+1]-b[j]);
			if(cross <= 0 && i < sz(a)-2) i++;
			if(cross >= 0 && j < sz(b)-2) j++;
		}
		return poly(sum);
	}
	
	// area ellipse = M_PI*a*b where a and b are the semi axis lengths
	// area triangle = sqrt(s*(s-a)(s-b)(s-c)) where s=(a+b+c)/2
	ld area(){ // O(n)
		ld area=0;
		forn(i, sz(pt)) area+=pt[i]^pt[(i+1)%sz(pt)];
		return abs(area)/ld(2);
	}
};