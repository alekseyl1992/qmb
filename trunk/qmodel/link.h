#ifndef H_LINK
#define H_LINK

namespace qmodel
{
	template<typename LHS_T, typename RHS_T>
	struct link
	{
		link(LHS_T v_lhs, RHS_T v_rhs) : lhs(v_lhs), rhs(v_rhs) { }
		~link() {  }
		
		LHS_T lhs;
		RHS_T rhs;
	};

} //end namespace qmodel
#endif // !H_LINK
