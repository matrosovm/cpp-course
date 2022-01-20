#pragma once

namespace cls_08 {
	template <class Ptr, class Type>
	inline void fill (Ptr ptr_begin, Ptr ptr_end, const Type& value) {
		Ptr it = ptr_begin;
		while (it != ptr_end) {
			*it = value;
			++it;
		} 
	}

	template <class Ptr, class GenerateFunc>
	inline void generate(Ptr ptr_begin, Ptr ptr_end, GenerateFunc generator) {
	    Ptr it = ptr_begin;
	    while (it != ptr_end) {
	        *it = generator();
	        ++it;
	    }
	}

	template <class SourcePtr, class DestinationPtr>
	inline void copy(SourcePtr source_begin, SourcePtr source_end, DestinationPtr destination_begin) {
        SourcePtr source_it = source_begin;
        DestinationPtr destination_it = destination_begin;
        while (source_it != source_end) {
            *destination_it = *source_it;
            ++source_it;
            ++destination_it;
        }
	}

    template <class Ptr, class Functor>
    inline void for_each(Ptr ptr_begin, Ptr ptr_end, Functor functor) {
        Ptr it = ptr_begin;
        while (it != ptr_end) {
            functor(*it);
            ++it;
        }
    }
}