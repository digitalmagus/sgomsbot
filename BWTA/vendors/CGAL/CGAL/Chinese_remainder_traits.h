// Copyright (c) 2006-2007 Max-Planck-Institute Saarbruecken (Germany).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.5-branch/Algebraic_foundations/include/CGAL/Chinese_remainder_traits.h $
// $Id: Chinese_remainder_traits.h 43073 2008-04-29 14:14:49Z hemmer $
//
//
// Author(s)     : Michael Hemmer    <hemmer@mpi-inf.mpg.de>
//
// =============================================================================


#ifndef CGAL_CHINESE_REMAINDER_TRAITS_H
#define CGAL_CHINESE_REMAINDER_TRAITS_H 1

#include <CGAL/basic.h>

#include <vector>

#include <CGAL/extended_euclidean_algorithm.h>

namespace CGAL{
namespace CGALi{

template <class T_, class TAG> 
class Chinese_remainder_traits_base{
public:
    typedef T_ Type;
    typedef ::CGAL::Null_tag     Scalar_type;
    typedef ::CGAL::Null_functor Chinese_remainder;
};
}

template <class T> class Chinese_remainder_traits
    :public CGALi::Chinese_remainder_traits_base<T,
       typename Algebraic_structure_traits<T>::Algebraic_category>{};


namespace CGALi {
template <class NT> 
class  Chinese_remainder_traits_base<NT,Euclidean_ring_tag>{
public:
    typedef NT Type;
    typedef NT Scalar_type; 
    
    struct Chinese_remainder{
        void operator() (
                const Scalar_type& m1, const Scalar_type& m2, const Scalar_type& m, 
                const Scalar_type& s,  const Scalar_type& t,  
                NT u1, NT u2, 
                NT& u) const {
#ifndef NDEBUG 
            NT tmp,s_,t_;
            tmp = CGAL::extended_euclidean_algorithm(m1,m2,s_,t_);
            CGAL_precondition(tmp == NT(1));
            CGAL_precondition(s_ == s);
            CGAL_precondition(t_ == t);
#endif

            typedef Algebraic_structure_traits<NT> AST;
            typename AST::Mod mod;
            //typename AST::Unit_part unit_part;
            typename AST::Integral_division idiv; 

            if(u1 < NT(0) ) u1 += m1;
            if(u2 < NT(0) ) u2 += m2;
            
            CGAL_precondition(0  < m1);
            CGAL_precondition(u1 < m1);
            CGAL_precondition(u1 >= NT(0));
        
            CGAL_precondition(0  < m2);
            CGAL_precondition(u2 < m2);
            CGAL_precondition(u2 >= NT(0));
    
            NT v = mod(s*(u2-u1),m2);
            u = m1*v + u1;
    
            // u is not unique yet!
            NT m_half = idiv(m-mod(m,NT(2)),NT(2));
            if (u  >  m_half) u -= m ;
            if (u <= -m_half) u += m ; 
        }
        
        void operator() (
                const Scalar_type& m1, const Type& u1,
                const Scalar_type& m2, const Type& u2,
                Scalar_type& m, Type& u) const {
            Scalar_type s,t; 
            
            CGAL::extended_euclidean_algorithm(m1,m2,s,t);
            m = m1 * m2;
            this->operator()(m1,m2,m,s,t,u1,u2,u);
        }
    };
};

} // namespace CGALi
} // namespace CGAL

#endif // CGAL_CHINESE_REMAINDER_TRAITS_H //

