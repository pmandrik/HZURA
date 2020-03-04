
#ifndef hzura_analyses_helpers_h
#define hzura_analyses_helpers_h 1

namespace hzura {

  // BASIC SELECTORS ===============================================================================================
  

  // COMBINATION OF PARTICLES ===============================================================================================
  // p1 + p2 => p
  template <typename A, typename B>
  void make_combination(const A & p1, const B & p2, Particle & p){
    p.tlv = p1.tlv + p2.tlv;
    p.mothers.push_back( (Particle*) (& p1) );
    p.mothers.push_back( (Particle*) (& p2) );
  }

  // p1 p2 p3 p4 p5 ...  -> p1+p2 p1+p3 ... p2+p3 p2+p4 ...
  template <typename A>
  void make_combinations(const vector<A> & inp,  vector<Particle> & out){
    int size = inp.size();
    Particle p;
    for( int i = 0; i < size; i++ ){
      for( int j = i+1; j < size; j++ ){
        make_combination( inp[i], inp[j], p );
        out.emplace_back( p );
      }
    }
  }

  template <typename A>
  void make_combinations_filtered( const vector<A> & inp, vector<Particle> & out, bool (*filter)(Particle *, Particle *) ){
    int size = inp.size();
    Particle p;
    for( int i = 0; i < size; i++ ){
      for( int j = i+1; j < size; j++ ){
        if( (*filter)( (Particle*) & inp[i], (Particle*) & inp[j] ) ) continue;
        make_combination( inp[i], inp[j], p );
        out.emplace_back( p );
      }
    }
  }

  template <typename A> void make_sameCharge_combinations( const vector<A> & inp, vector<Particle> & out ){
    make_combinations_filtered(inp, out, [](Particle * a, Particle * b) { return ((int)a->Charge() != (int)b->Charge()); } );
  }

  template <typename A> void make_diffCharge_combinations( const vector<A> & inp, vector<Particle> & out ){
    make_combinations_filtered(inp, out, [](Particle * a, Particle * b) { return ((int)a->Charge() == (int)b->Charge()); } );
  }

  // (p11 p12 p13 p14 p15 ... ) (p21 p22 p23 p24 p25 ... ) -> p11+p21 p11+p22 ... p12+p21 p12+p21 ...
  template <typename A, typename B>
  void make_combinations(const vector<A> & inp_1, const vector<B> & inp_2, vector<Particle> & out){
    Particle p;
    for( int i = 0, size_1 = inp_1.size(); i < size_1; i++ ){
      for( int j = 0, size_2 = inp_2.size(); j < size_2; j++ ){
        make_combination( inp_1[i], inp_2[j], p );
        out.emplace_back( p );
      }
    }
  }

  template <typename A, typename B>
  void make_combinations_filtered(const vector<A> & inp_1, const vector<B> & inp_2, vector<Particle> & out, bool (*filter)(Particle *, Particle *)  ){
    Particle p;
    for( int i = 0, size_1 = inp_1.size(); i < size_1; i++ ){
      for( int j = 0, size_2 = inp_2.size(); j < size_2; j++ ){
        if( (*filter)( (Particle*) & inp_1[i], (Particle*) & inp_2[j] ) ) continue;
        make_combination( inp_1[i], inp_2[j], p );
        out.emplace_back( p );
      }
    }
  }

  template <typename A, typename B> void make_sameCharge_combinations( const vector<A> & inp_1, const vector<B> & inp_2, vector<Particle> & out ){
    make_combinations_filtered(inp_1, inp_2, out, [](Particle * a, Particle * b) { return (a->Charge() != b->Charge()); } );
  }

  template <typename A, typename B> void make_diffCharge_combinations( const vector<A> & inp_1, const vector<B> & inp_2, vector<Particle> & out ){
    make_combinations_filtered(inp_1, inp_2, out, [](Particle * a, Particle * b) { return (a->Charge() == b->Charge()); } );
  }

};

#endif






