/* Copyright 2004
   Stanford University

   This file is part of the DSR PDB Library.

   The DSR PDB Library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation; either version 2.1 of the License, or (at your
   option) any later version.

   The DSR PDB Library is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
   License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with the DSR PDB Library; see the file LICENSE.LGPL.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA. */

#ifndef DSR_MODEL_H_
#define DSR_MODEL_H_
#include <CGAL/PDB/Chain.h>
#include <vector>
#include <string>
#include <boost/tuple/tuple.hpp>

CGAL_PDB_BEGIN_NAMESPACE

class PDB;

//! A class representing a single model from a PDB file.
/*!
  You can iterator through the chains and soon the heterogens. 
*/
class Model {
  friend class PDB;

  CGAL_SMALL_MAP_VALUE_TYPE(Model_vt, CGAL::Label<Model>, Chain, chain); 

  typedef small_map<Model_vt> Chains; 
public:
  //! Construct an empty model
  Model();

  
  typedef CGAL::Label<Model> Chain_key;

  void swap_with(Model &o);

  //! write to a pdb
  void write(int model_index, std::ostream &out) const;
  
  //! write screen
  std::ostream& write(std::ostream &out) const{
    write(0,out);
    return out;
  }


  //! Iterator through the chains
  CGAL_PDB_ITERATOR(Chain, chain, Chains::iterator, 
		    return chains_.begin(), return chains_.end());

 //! Iterator through the chains
  CGAL_PDB_CONST_ITERATOR(Chain, chain, Chains::const_iterator, 
			  return chains_.begin(), return chains_.end());


  //! get the chain identified by c
  CGAL_PDB_FIND(Chain, return chains_.find(k));

  CGAL_PDB_INSERT(Chain, return chains_.insert(Chains::value_type(k, m)););

  CGAL_PDB_SIZE(chains, return chains_.size());

  //! \cond
  class Hetatom_data {
  public:
    Hetatom_data(const char *rnm, 
		 const char *anm, int rn, char ch): resname_(rnm),
						    atomname_(anm),
						    rnum_(rn), chain_(ch){
    }
    Hetatom_data(){}
    const char *molecule_name() const {
      return resname_.c_str();
    }
    const char *atom_name() const {
      return atomname_.c_str();
    }
    int molecule_number() const {
      return rnum_;
    }
    Chain_key chain() const {
      return chain_;
    }
    bool operator<(const Hetatom_data &o) const {
      CGAL_PDB_IFNONEQUAL(rnum_, o.rnum_, <)
      else CGAL_PDB_IFNONEQUAL(chain_, o.chain_, <)
      else return false;
    }
    bool operator>(const Hetatom_data &o) const {
      CGAL_PDB_IFNONEQUAL(rnum_, o.rnum_, >)
      else CGAL_PDB_IFNONEQUAL(chain_, o.chain_, >)
      else return false;
    }
    bool operator==(const Hetatom_data &o) const {
      return rnum_==o.rnum_ && chain_==o.chain_;
    }
    bool operator!=(const Hetatom_data &o) const {
      return rnum_!=o.rnum_ || chain_!=o.chain_;
    }
  protected:
    std::string resname_;
    std::string atomname_;
    int rnum_;
    Chain_key chain_;
      
  };
  //! \endcond


  CGAL_SMALL_MAP_VALUE_TYPE(Hetatom_vt, Hetatom_data, Atom, atom);
  //! \cond
  typedef std::vector<Hetatom_vt > Hetatoms;
  //! \endcond

  //! An iterator through CGAL::PDB::Atom values for the HETATM records.
  CGAL_PDB_CONST_ITERATOR(Hetatom, hetatom, 
			  Hetatoms::const_iterator,
			  return hetatoms_.begin(),
			  return hetatoms_.end());


  //! The number of hetatoms
  CGAL_PDB_SIZE(hetatoms, return hetatoms_.size());






  //! A unique identified of an atom in the Model
  struct Atom_key: public boost::tuple<Chain_key, Chain::Monomer_key, 
				       Monomer::Atom_key>{
    typedef boost::tuple<Chain_key, Chain::Monomer_key, Monomer::Atom_key> P;
    Atom_key(Chain_key ck, Chain::Monomer_key mk, Monomer::Atom_key at): P(ck, mk, at){}
    Atom_key(){}
    operator Chain_key() const {
      return chain_key();
    }
    operator Monomer::Atom_key() const {
      return atom_key();
    }
    operator Chain::Monomer_key() const {
      return monomer_key();
    }
    Monomer::Atom_key atom_key() const {
      return P::get<2>();
    }
    Chain::Monomer_key monomer_key() const {
      return P::get<1>();
    }
    Chain_key chain_key() const {
      return P::get<0>();
    }
  };



  //! The endpoint of a bond
  class Bond_endpoint {
    const Atom *atom_;
    Atom_key ak_;
    friend class Bond_it;
 public:
    Bond_endpoint(Atom_key ak, const Atom *atom): atom_(atom), ak_(ak){}    
    Bond_endpoint():atom_(NULL){}
    const Atom &atom() const {return *atom_;}
    Atom_key key() const {return ak_;}
  };


  //! A chemical bond within the protein
  typedef std::pair<Bond_endpoint, Bond_endpoint> Bond; 
  class Atom_iterator_value_type {
      Atom_key index_;
      Atom *atom_;
  public:
      Atom_iterator_value_type(Atom_key f, Atom* s): index_(f), atom_(s){}
      Atom_key key() const {return index_;}
      Atom &atom() const {return *atom_;}
      Atom_iterator_value_type():atom_(NULL){}
    };
  class Atom_const_iterator_value_type {
      Atom_key index_;
      const Atom *atom_;
  public:
      Atom_const_iterator_value_type(Atom_key f, const Atom* s): index_(f), atom_(s){}
      Atom_key key() const {return index_;}
      const Atom &atom() const {return *atom_;}
      Atom_const_iterator_value_type():atom_(NULL){}
    };
protected:
  //! \cond
 struct Iterator_traits {
    typedef Chain_iterator Outer_it;
    typedef Chain::Atom_iterator Inner_it;
   typedef Atom_iterator_value_type value_type;
    struct Inner_range{
      std::pair<Inner_it, Inner_it> operator()(Outer_it it) const {
	return std::make_pair(it->chain().atoms_begin(), it->chain().atoms_end());
      }
    };
    struct Make_value{
      value_type operator()(Outer_it oit, Inner_it iit) const {
	return value_type(Atom_key(oit->key(), iit->key().monomer_key(),
				   iit->key().atom_key()), &iit->atom());
      }
    };
  };


  struct Iterator_const_traits {
    typedef Chain_const_iterator Outer_it;
    typedef Chain::Atom_const_iterator Inner_it;
    typedef Atom_const_iterator_value_type value_type;
   struct Inner_range{
      std::pair<Inner_it, Inner_it> operator()(Outer_it it) const {
	return std::make_pair(it->chain().atoms_begin(), it->chain().atoms_end());
      }
    };
    struct Make_value{
      value_type operator()(Outer_it oit, Inner_it iit) const {
	return value_type(Atom_key(oit->key(), iit->key().monomer_key(),
				   iit->key().atom_key()), &iit->atom());
      }
    };
  };
  //! \endcond
public:
 //! An iterator to iterate through all the atoms of the protein  
  CGAL_PDB_ITERATOR(Atom, atom, 
		    internal::Nested_iterator<Iterator_traits >,
		    return Atom_iterator(chains_.begin(), chains_.end()),
		    return Atom_iterator(chains_.end(), chains_.end()));
  //! An iterator to iterate through all the atoms of the protein  
  CGAL_PDB_CONST_ITERATOR(Atom, atom, 
		    internal::Nested_iterator<Iterator_const_traits >,
		    return Atom_const_iterator(chains_.begin(), chains_.end()),
		    return Atom_const_iterator(chains_.end(), chains_.end()));


  //! \cond
  class Bond_it {
    friend class Model;
  public:
    //! The value_type is a CGAL::PDB::Chain::Bond
    typedef Bond value_type;
    typedef std::forward_iterator_tag iterator_category;
    typedef std::size_t difference_type;
    typedef const Bond& reference;
    typedef const Bond* pointer;
    
    reference operator*() const {
      return ret_;
    }
    pointer operator->() const {
      return &ret_;
    }
    Bond_it operator++() {
      ++ait_;
      while (ait_== rit_->chain().bonds_end()) {
	++rit_;
	if (rit_!= rend_) {
	  ait_= rit_->chain().bonds_begin();
	} else {
	  return *this;
	}
      }
      make_bond();
      return *this;
    }
    bool operator==(const Bond_it& o) const {
      if (rit_ == rend_) return rit_==o.rit_;
      else return rit_== o.rit_ && ait_ == o.ait_;
    }
  bool operator!=(const Bond_it& o) const {
      return !operator==(o);
    }
    
    Bond_it(){}

    CGAL_PDB_COPY_CONSTRUCTOR(Bond_it);

  
  protected:
    void copy_from(const Bond_it &o) {
      rit_= o.rit_;
      rend_= o.rend_;
      if (rit_!= rend_) {
	ait_= o.ait_;
	ret_= o.ret_;
      }
    }

    Bond_it(Chain_const_iterator b, 
	    Chain_const_iterator e): rit_(b), rend_(e){
      if (b != e) {
	ait_= rit_->chain().bonds_begin();
	make_bond();
      }
    }

    void make_bond() {
      ret_= Bond(Bond_endpoint(Atom_key(rit_->key(), 
					ait_->first.key().monomer_key(),
					ait_->first.key().atom_key()),
			       &ait_->first.atom()),
		 Bond_endpoint(Atom_key(rit_->key(), 
					ait_->second.key().monomer_key(),
					ait_->second.key().atom_key()),
			       &ait_->second.atom())); 
    }

    Chain_const_iterator rit_, rend_;
    Chain::Bond_const_iterator ait_;
    Bond ret_;
  };
  //! \endcond

  CGAL_PDB_CONST_ITERATOR(Bond, bond, Bond_it,
			  return Bond_const_iterator(chains_.begin(),
						     chains_.end()),
			  return Bond_const_iterator(chains_.end(), 
						     chains_.end()));
 

private:
  void process_line(const char *c);

  std::vector<std::string> extra_;
  Chains chains_;
  Hetatoms hetatoms_;
};

CGAL_PDB_SWAP(Model);

CGAL_PDB_OUTPUT(Model);

//! Assign unique indices to all atoms in the Model, starting at optional start value
/*!
  This returns the next unused index. 
*/
inline int index_atoms(const Model &c, int start=0) {
  for (Model::Chain_const_iterator it= c.chains_begin(); it != c.chains_end(); ++it) {
    start= index_atoms(it->chain(), start);
  }
  return start;
}

CGAL_PDB_END_NAMESPACE
#endif
