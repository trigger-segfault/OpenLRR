// ListSet.hpp : C++ template utilities for a commmon type of pool/container used in LegoRR.
//

#pragma once

#include "../../common.h"

#include "Errors.h"
#include "Memory.h"


/// TEMPLATE REQUIREMENTS:
// Note that the order of these fields is not important.
// 
// struct TCont {
//     TItem* listSet[MAXLISTS];
//     TItem* freeList;
//     uint32 listCount;
// };
// 
// struct TItem {
//     TItem* nextFree; // Normally the last field, but there isn't any hard requirement.
// };
//
// bool FPredicate(const TItem* item);


namespace ListSet
{; // !<---

#pragma region ListSet Template Helpers

/**
 * @brief Template to obtain a listSet container type's element type.
 * @param TCont The container type holding a listSet.
 */
template <class TCont>
using container_value_t = typename std::remove_pointer_t<std::remove_extent_t<decltype(TCont::listSet)>>;

/**
 * @brief Template filter function typedef for an item value_type.
 */
template <class TItem>
using predicate_f = bool (const TItem*);

#pragma endregion


#pragma region ListSet Math

/**
 * @brief Returns the maximum number of lists in a type's listSet.
 * @param TCont The container type holding a listSet.
 * @return The count of TCont's C array listSet.
 */
template <typename TCont>
constexpr const size_t MaxLists()
{
	return _countof(TCont::listSet);
}

/**
 * @brief Returns the total number of items available in a listSet with N lists.
 * @param listCount The number of lists in a listSet (total or currently allocated).
 * @return The combined size of listCount lists.
 */
constexpr const size_t CapacityOfLists(size_t listCount)
{
	return ((static_cast<size_t>(1U) << listCount) - 1U);
}

/**
 * @brief Returns the number of items available in a single list at index N in a listSet.
 * @param listIndex The index of the list in a listSet.
 * @return The combined size of listCount lists.
 */
constexpr const size_t CountOfList(size_t listIndex)
{
	return (static_cast<size_t>(1U) << listIndex);
}

/**
 * @brief Returns the index of an item in a specific listSet list.
 *        Should only used when the item is known to exist in the specified list.
 * @param listIndex The index of the list in a listSet.
 * @return The index of the item from the start of the list (calculated with pointer math).
 */
template <typename TItem>
constexpr const size_t IndexOfInList(const TItem* list, const TItem* item)
{
	return (static_cast<size_t>(item - list) / sizeof(TItem));
}

/**
 * @brief Returns the absolute index of an item in a listSet.
 * @param listIndex The index of the list in a listSet that holds the item.
 * @param itemIndex The index of the item in the list.
 * @return The itemIndex + the capacity of all previous lists.
 */
constexpr const size_t IndexOfInListSet(size_t listIndex, size_t itemIndex)
{
	// Capacity of all previous lists + index in current list
	return ListSet::CapacityOfLists(listIndex) + itemIndex;
}

/**
 * @brief Returns the absolute index of an item in a listSet.
 * @param listIndex The index of the list in a listSet that holds the item.
 * @param list The pointer to the list at listIndex.
 * @param item The pointer to the item in list.
 * @return The itemIndex + the capacity of all previous lists.
 */
template <typename TItem>
constexpr const size_t IndexOfInListSet(size_t listIndex, const TItem* list, const TItem* item)
{
	return ListSet::IndexOfInListSet(listIndex, ListSet::IndexOfInList(list, item));
}

#pragma endregion


#pragma region ListSet Assign

/**
 * @brief Calls `std::memset(item, 0, sizeof(TItem))`, while preserving the nextFree field.
 * @param TItem The value_type of a listSet container.
 * @param item The listSet item to assign the memory of.
 */
template <typename TItem>
void MemZero(TItem* item)
{
	TItem* tmpNextFree = item->nextFree;
	std::memset(item, 0, sizeof(TItem));
	item->nextFree = tmpNextFree;
}

/**
 * @brief Calls `std::memcpy(dest, src, sizeof(TItem))`, while preserving the nextFree field of dest.
 * @param TItem The value_type of a listSet container.
 * @param dest The listSet item to assign the memory of.
 * @param src The listSet item to copy the memory of.
 */
template <typename TItem>
void MemCopy(TItem* dest, const TItem* src)
{
	TItem* tmpNextFree = dest->nextFree;
	std::memcpy(dest, src, sizeof(TItem));
	dest->nextFree = tmpNextFree;
}

/**
 * @brief Calls the assignment operator `*dest = *src`, while preserving the nextFree field of dest.
 * @param TItem The value_type of a listSet container.
 * @param dest The listSet item on the left-hand side of the assignment operator.
 * @param src The listSet item on the right-hand side of the assignment operator.
 */
template <typename TItem>
void Assign(TItem* dest, const TItem* src)
{
	TItem* tmpNextFree = dest->nextFree;
	*dest = *src; // Use assignment operator in case TItem wants to overload this in the future.
	dest->nextFree = tmpNextFree;
}

#pragma endregion


#pragma region ListSet Item State

/**
 * @brief Tests if a listSet item is alive.
 * @param TItem The listSet item type.
 * @param item An item created by the listSet.
 * @return True if the item is alive, and managed by the user.
 */
template <typename TItem>
bool IsAlive(const TItem* item)
{
	return (item->nextFree == item);
}

/**
 * @brief Tests if a listSet item is dead.
 * @param TItem The listSet item type.
 * @param item An item created by the listSet.
 * @return True if the item is dead, and managed by the listSet.
 */
template <typename TItem>
bool IsDead(const TItem* item)
{
	return (item->nextFree != item);
}

/**
 * @brief Tests if a listSet item is null or dead (shorthand for !item && IsDead(item)).
 * @param TItem The listSet item type.
 * @param item An item created by the listSet.
 * @return True if the item is null, or is dead, and managed by the listSet.
 */
template <typename TItem>
bool IsNullOrDead(const TItem* item)
{
	return (!item || item->nextFree != item);
}

#pragma endregion


#pragma region ListSet Filter Functions

/**
 * @brief A listSet filter function to return all items, alive or dead.
 * @param TItem The value_type of a listSet container.
 * @param item An item enumerated over in the listSet.
 * @return True.
 */
template <typename TItem>
bool NoFilter(const TItem* item) { return true; }

/**
 * @brief A listSet filter function to only return items that are alive, and managed by the user.
 * @param TItem The value_type of a listSet container.
 * @param item An item enumerated over in the listSet.
 * @return True if the item is alive in the listSet, otherwise false.
 */
template <typename TItem>
bool AliveFilter(const TItem* item) { return ListSet::IsAlive(item); }

/**
 * @brief A listSet filter function to only return items that are dead, and managed by the listSet.
 * @param TItem The value_type of a listSet container.
 * @param item An item enumerated over in the listSet.
 * @return False if the item is alive in the listSet, otherwise true.
 */
template <typename TItem>
bool DeadFilter(const TItem* item) { return !ListSet::IsAlive(item); }

#pragma endregion


#pragma region ListSet Iterator

/**
 * @brief The base iterator type for ListSet collections. (use Iterator or ReverseIterator)
 * @param TCont The container type holding a listSet.
 * @param FPredicate A filter function to skip past items where false is returned.
 * @param Reverse Only use in combination with std::reverse_iterator<>.
 *        Changes the search direction for the first valid item during the constructor.
 */
template <typename TCont, const predicate_f<container_value_t<TCont>> FPredicate, const bool Reverse>
class BaseIterator
{
public:
	using container         = TCont;
	using difference_type   = std::ptrdiff_t;
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type        = container_value_t<container>;
	using iterator          = BaseIterator<container, FPredicate, Reverse>;


	static constexpr size_t npos = static_cast<size_t>(-1);


	/**
	 * @brief Construct an iterator at the specified list/item index in a listSet.
	 * @param cont The listSet container (i.e. Config_Globs configGlobs).
	 * @param listIndex The index of the list in cont's listSet that holds the item.
	 * @param itemIndex The index of the item in the list.
	 */
	BaseIterator(const container& cont, ptrdiff_t listIndex, ptrdiff_t itemIndex)
		: m_cont(cont), m_listIndex(listIndex), m_itemIndex(0), m_itemCount(0), m_ptr(nullptr)
	{
		// Only perform validation (and assign itemIndex) if we're inside the listSet bounds.
		// Otherwise this is a terminator iterator: i.e. end() or rend()
		if (listIndex >= 0 && listIndex < static_cast<ptrdiff_t>(ListSet::MaxLists<container>())) {

			// Pre-check for null list:

			// When a list is null, inner count and index are 0 so that we instantly skip during increment/decrement.
			value_type* list = m_cont.listSet[listIndex];

			// Also check that we're searching within the bounds of listCount.
			// The bounds check for listCount is placed here rather than where MaxLists is in order to:
			// - increment up to end() (aka MaxLists) in the case of a forward iterator.
			// - decrement to the next valid item in the case of a reverse iterator.
			if (list && listIndex < static_cast<ptrdiff_t>(m_cont.listCount)) {
				m_itemCount = ListSet::CountOfList(listIndex);
				m_itemIndex = (itemIndex >= 0 ? itemIndex : (m_itemCount + itemIndex));

				m_ptr        = &list[m_itemIndex];
			}

			// Go to our first valid item:

			// Null pointer means we automatically need to check for the next.
			// But we also need to filter towards our first valid item, even when non-null.
			if (!m_ptr || !FPredicate(m_ptr)) {
				if (Reverse) --(*this);
				else         ++(*this);
			}
		}
	}


	// Iterator evaluation operator requirements:

	/**
	 * @brief Returns a pointer to the current item in the listSet.
	 *        Note that operator `It->` also returns the same pointer level.
	 * @return A pointer to the current item.
	 */
	value_type* operator*()  const { return m_ptr; }
	/**
	 * @brief Returns a pointer to the current item in the listSet.
	 *        Note that operator `*It` also returns the same pointer level.
	 * @return A pointer to the current item to be accessed.
	 */
	value_type* operator->() const { return m_ptr; }


	// Forward directional increment operator requirements:

	// Prefix increment
	iterator& operator++()
	{
		do {
			// Increment our item index and check if we need to move to the next list.
			if (++m_itemIndex >= static_cast<ptrdiff_t>(m_itemCount)) {

				// Find next non-null list, and terminate inside if we reach the end of the listSet.
				do {
					// We can safely skip any remaining unused lists here.
					if (++m_listIndex >= static_cast<ptrdiff_t>(m_cont.listCount)) {
						// Always jump to end at MaxLists, rather than listCount.
						// This way we can preserve Gods98 list iteration,
						//  where foreach loops will always iterate over newly-appended lists as it goes.
						m_listIndex = ListSet::MaxLists<container>();
						m_itemIndex = 0;
						m_itemCount = 0;
						m_ptr       = nullptr;
						return *this; // end of listSet
					}

					// We want to skip null lists. Realistically all remaining lists would also be null.
				} while (!m_cont.listSet[m_listIndex]);

				m_itemCount = ListSet::CountOfList(m_listIndex);
				m_itemIndex = 0;
			}

			// Update our pointer to the current item. (alternatively can be done after do-while loop)
			m_ptr = &m_cont.listSet[m_listIndex][m_itemIndex];

			// Can we return our current item? (generally used to check if item is alive in the listSet).
		} while (!FPredicate(m_ptr));

		return *this;
	}

	// Postfix increment
	iterator operator++(int)
	{
		iterator tmp = *this;
		++(*this);
		return tmp;
	}


	// Bidirectional iterator decrement operator requirements:

	// Prefix decrement
	iterator& operator--()
	{
		do {
			// Decrement our item index and check if we need to move to the previous list.
			if (m_itemIndex-- <= 0) {

				// Find previous non-null list and terminate inside if we reach the beginning of the listSet.
				do {
					if (m_listIndex-- <= 0) {
						// Always end at list -1, to signal we've reached the beginning of the listSet.
						m_listIndex = -1;
						m_itemIndex = 0;
						m_itemCount = 0;
						m_ptr       = nullptr;
						return *this; // pre-beginning of listSet
					}

					// We want to skip null lists, and also skip anything after listCount.
				} while (!m_cont.listSet[m_listIndex] || m_listIndex >= static_cast<ptrdiff_t>(m_cont.listCount));

				m_itemCount = ListSet::CountOfList(m_listIndex);
				m_itemIndex = (m_itemCount - 1);
			}

			// Update our pointer to the current item. (could alternatively be done after the do-while loop)
			m_ptr = &m_cont.listSet[m_listIndex][m_itemIndex];

			// Can we return our current item? (generally used to check if item is alive in the listSet).
		} while (!FPredicate(m_ptr));

		return *this;
	}

	// Postfix decrement
	iterator operator--(int)
	{
		iterator tmp = *this;
		--(*this);
		return tmp;
	}


	/**
	 * @brief Gets the index of the current list in the listSet.
	 */
	ptrdiff_t ListIndex() const { return m_listIndex; }
	/**
	 * @brief Gets the index of the current item within the current list.
	 */
	ptrdiff_t ItemIndex() const { return m_itemIndex; }

	/**
	 * @brief Gets the absolute index of the current item in the listSet.
	 * @return The absolute index of the current item, or npos if an item is not currently pointed to (e.g. end()).
	 */
	size_t Index() const { return (!m_ptr ? npos : ListSet::IndexOfInListSet(m_listIndex, m_itemIndex)); }

	/**
	 * @brief Tests if the current item is alive, and managed by the user.
	 */
	bool IsAlive() const { return ListSet::IsAlive(m_ptr); }
	/**
	 * @brief Tests if the current item is dead, and managed by the listSet.
	 */
	bool IsDead()  const { return !ListSet::IsAlive(m_ptr); }


	// Foreach comparison operator requirements:

	friend bool operator== (const iterator& a, const iterator& b) { return a.m_itemIndex == b.m_itemIndex && a.m_listIndex == b.m_listIndex; };
	friend bool operator!= (const iterator& a, const iterator& b) { return a.m_itemIndex != b.m_itemIndex || a.m_listIndex != b.m_listIndex; };


protected:
	const container& m_cont;
	ptrdiff_t m_listIndex; // Index of the current list in the listSet.
	ptrdiff_t m_itemIndex; // Index of the current item in the list.
	size_t m_itemCount;    // Size of the current list.
	value_type* m_ptr;     // Pointer to the current item.
};


/**
 * @brief A forward iterator type for ListSet collections.
 * @param TCont The container type holding a listSet.
 * @param FPredicate A filter function to skip past items where false is returned.
 */
template <typename TCont, const predicate_f<container_value_t<TCont>> FPredicate>
using Iterator = BaseIterator<TCont, FPredicate, false>;

/**
 * @brief A reverse iterator type for ListSet collections.
 * @param TCont The container type holding a listSet.
 * @param FPredicate A filter function to skip past items where false is returned.
 */
template <typename TCont, const predicate_f<container_value_t<TCont>> FPredicate>
using ReverseIterator = std::reverse_iterator<BaseIterator<TCont, FPredicate, true>>;

#pragma endregion


#pragma region ListSet Enumerable

/**
 * @brief Template class for iterating over a listSet container's elements in a foreach loop.
 * @param TCont The container type holding a listSet.
 * @param FPredicate A filter function to skip past items where false is returned.
 */
template <typename TCont, const predicate_f<container_value_t<TCont>> FPredicate>
class Enumerable
{
public:
	using container         = TCont;
	using value_type        = container_value_t<container>;
	using iterator          = Iterator<container, FPredicate>;
	using reverse_iterator  = ReverseIterator<container, FPredicate>;


	static constexpr size_t npos = iterator::npos;


	/**
	 * @brief Constructs an enumerable collection wrapper around an existing listSet struct.
	 * @param cont The listSet container (i.e. Config_Globs configGlobs).
	 */
	Enumerable(container& cont)
		: m_cont(cont)
	{
	}


	/**
	 * @brief Gets the number of lists currently allocated in the listSet.
	 */
	size_t ListCount() const { return m_cont.listCount; }

	/**
	 * @brief Gets the total number of lists that can be allocated in the listSet.
	 */
	constexpr size_t const MaxLists() const { return ListSet::MaxLists<container>(); }

	/**
	 * @brief Returns the capacity of the currently allocated lists in the listSet.
	 */
	size_t Capacity() const { return ListSet::CapacityOfLists(m_cont.listCount); }

	/**
	 * @brief Returns the total capacity of the listSet if all lists are currently allocated.
	 */
	constexpr size_t const MaxCapacity() const { return ListSet::CapacityOfLists(ListSet::MaxLists<container>); }

	/**
	 * @brief Returns the number of valid items in the listSet enumerable.
	 */
	size_t Count() { return static_cast<size_t>(std::distance(this->begin(), this->end())); }


	iterator begin()              { return iterator(m_cont, 0, 0); }
	const iterator cbegin() const { return iterator(m_cont, 0, 0); }

	iterator end()                { return iterator(m_cont, ListSet::MaxLists<container>(), 0); }
	const iterator cend()   const { return iterator(m_cont, ListSet::MaxLists<container>(), 0); }

	reverse_iterator rbegin()              { return reverse_iterator(m_cont, m_cont.listCount - 1, -1); }
	const reverse_iterator crbegin() const { return reverse_iterator(m_cont, m_cont.listCount - 1, -1); }

	reverse_iterator rend()                { return reverse_iterator(m_cont, -1, -1); }
	const reverse_iterator crend()   const { return reverse_iterator(m_cont, -1, -1); }


protected:
	container& m_cont;
};

/**
 * @brief Template class for iterating over a listSet container's elements in a foreach loop (with no filter).
 * @param TCont The container type holding a listSet.
 */
template <typename TCont>
using DefaultEnumerable = Enumerable<TCont, NoFilter<container_value_t<TCont>>>;

#pragma endregion


#pragma region ListSet WrapperCollection

/**
 * @brief Template class for managing the listSet collection of a wrapped structure.
 * @param TCont The container type holding a listSet.
 */
template <typename TCont>
class WrapperCollection : public DefaultEnumerable<TCont>
{
public:
	using container         = TCont;
	using value_type        = container_value_t<container>;

	template <const predicate_f<value_type> FPredicate>
	using enumerable        = Enumerable<container, FPredicate>;


	using DefaultEnumerable<TCont>::m_cont; // Included so that `m_cont` member can be accessed without `this->`.
	using DefaultEnumerable<TCont>::npos;


	/**
	 * @brief Constructs a listSet collection manager wrapped around an existing listSet struct.
	 * @param cont The listSet container (i.e. Config_Globs configGlobs).
	 */
	WrapperCollection(container& cont)
		: DefaultEnumerable<TCont>(cont), m_aliveCount(0)
	{
	}


	/**
	 * @brief Returns the index of the list containing this item in the listSet.
	 * @param item The item to find the list of in the listSet.
	 * @return The list index containing this item in the listSet, or npos if not found.
	 */
	size_t ListIndexOf(const value_type* item)
	{
		for (size_t i = 0; i < m_cont.listCount; i++) {
			const value_type* list = m_cont.listSet[i];
			if (list && (item >= list && item < &list[ListSet::CountOfList(i)])) {
				return i;
			}
		}
		return npos;
	}

	/**
	 * @brief Returns the absolute index of the item in the listSet.
	 * @param item The item to find in the listSet.
	 * @return The index of the item in its list + the capacity of all previous lists in the listSet, or npos if not found.
	 */
	size_t IndexOf(const value_type* item)
	{
		const size_t listIndex = this->ListIndexOf(item);
		if (listIndex != npos) {
			const size_t itemIndex = ListSet::IndexOfInList(m_cont.listSet[listIndex], item);
			return ListSet::IndexOfInListSet(listIndex, itemIndex);
		}
		return npos;
	}


	/**
	 * @brief Returns an enumerable for iterating over filtered items in the listSet.
	 * @param FPredicate A filter function to skip past items where false is returned.
	 */
	template <const predicate_f<value_type> FPredicate>
	enumerable<FPredicate> EnumerateWhere() { return enumerable<FPredicate>(m_cont); }

	/**
	 * @brief Returns an enumerable for iterating over alive items in the listSet.
	 */
	enumerable<AliveFilter<value_type>> EnumerateAlive() { return enumerable<AliveFilter<value_type>>(m_cont); }

	/**
	 * @brief Returns an enumerable for iterating over dead items in the listSet.
	 */
	enumerable<DeadFilter<value_type>> EnumerateDead() { return enumerable<DeadFilter<value_type>>(m_cont); }

	/**
	 * @brief Returns an enumerable for iterating over every item in the listSet.
	 */
	enumerable<NoFilter<value_type>> Enumerate() { return enumerable<NoFilter<value_type>>(m_cont); }


	/**
	 * @brief Returns the number of items that are currently alive in the listSet.
	 */
	size_t CountAlive() { return m_aliveCount; /*EnumerateAlive().Count();*/ }


	/**
	 * @brief Performs initial listSet setup by zeroing all appropriate fields.
	 */
	void Initialise()
	{
		for (size_t i = 0; i < this->MaxLists(); i++) {
			m_cont.listSet[i] = nullptr;
		}
		m_cont.listCount = 0;
		m_cont.freeList = nullptr;
	}

	/**
	 * @brief Performs listSet cleanup by freeing allocated lists and zeroing all appropriate fields.
	 *        Note that the user should handle cleanup of individual list items beforehand
	 *        (but `Remove` does not need to be called).
	 */
	void Shutdown()
	{
		for (size_t i = 0; i < this->MaxLists(); i++) {
			if (m_cont.listSet[i]) {
				Gods98::Mem_Free(m_cont.listSet[i]);
				m_cont.listSet[i] = nullptr;
			}
		}

		// NOTE: LegoRR ListSet shutdowns often do not reset one or both these fields.
		m_cont.listCount = 0;
		m_cont.freeList = nullptr;
	}

	/**
	 * @brief Reserves and returns a new item in the listSet.
	 * @return A new living item in the listSet.
	 */
	value_type* Add()
	{
		// Add a new list to the listSet if we've run out of free items.
		if (m_cont.freeList == nullptr) this->AddList();

		value_type* newItem = m_cont.freeList;

		Error_Fatal(!newItem, "NULL found in ListSet::freeList");
		Error_Fatal(ListSet::IsAlive(newItem), "Alive item found in ListSet::Add");

		// Splice our item out of the linked list of free items.
		m_cont.freeList = newItem->nextFree;

		// Mark our item as alive by assigning its linked list next free pointer to itself.
		newItem->nextFree = newItem;

		m_aliveCount++;

		return newItem;
	}

	/**
	 * @brief Remove a living item from the listSet.
	 * @param dead An item previously created by the listSet that has yet to be removed.
	 */
	void Remove(value_type* dead)
	{
		Error_Fatal(!dead, "NULL passed to ListSet::Remove");
		Error_Fatal(!ListSet::IsAlive(dead), "Dead item passed to ListSet::Remove");

		m_aliveCount--;

		// Reconnect our dead item to the listSet by appending it to the linked list of free items.
		dead->nextFree = m_cont.freeList;
		m_cont.freeList = dead;
	}

	/**
	 * @brief Adds a new list to the listSet. This is automatically called by `Add` when `freeList == nullptr`.
	 */
	void AddList()
	{
		Error_Fatal(m_cont.listCount + 1 >= this->MaxLists(), "Run out of lists in ListSet::AddList");

		const size_t count = ListSet::CountOfList(m_cont.listCount);

		value_type* list = m_cont.listSet[m_cont.listCount] = (value_type*)Gods98::Mem_Alloc(sizeof(value_type) * count);
		if (list) {
			m_cont.listCount++;

			// Connect all items in a forward-linked-list fashion.
			for (size_t i = 1; i < count; i++) {

				list[i - 1].nextFree = &list[i];
			}

			// Finally, connect our newly allocated list to the remainder of the listSet.
			// NOTE: freeList is always null when calling AddList (under normal circumstances).
			list[count - 1].nextFree = m_cont.freeList;
			m_cont.freeList = list;

		}
		else Error_Fatal(true, Gods98::Error_Format("Unable to allocate %d bytes of memory for new list.\n", sizeof(value_type) * count));
	}


protected:
	/**
	 * @brief Unallocates the last list in the listSet, and decrements listCount.
	 */
	void PopList()
	{
		Error_Fatal(m_cont.listCount == 0, "Cannot pop with zero lists in ListSet::PopList");

		const size_t listIndex = m_cont.listCount - 1;

		const value_type* list = m_cont.listSet[listIndex];
		if (list) {
			const value_type* listEnd = &list[ListSet::CountOfList(listIndex)];

			// Before unallocating the list memory, we need to disconnect its items
			// from the forward-linked list of free items (pointed to by freeList and nextFree).
			value_type* prevFree = nullptr;

			for (value_type* item = m_cont.freeList; item != nullptr; item = item->nextFree) {

				// Is this item not contained in the list being removed?
				if (item < list || item >= listEnd) {
					// If so, then connect it to our previous free item, and make this our new previous item to reconnect.
					if (prevFree) prevFree->nextFree = item;
					else          m_cont.freeList    = item;
					prevFree = item;
				}
				// Otherwise we can skip over this item without having to change anything.
			}

			// Finish the reconnection loop by pointing our final free item to null.
			if (prevFree) prevFree->nextFree = nullptr;
			else          m_cont.freeList    = nullptr;


			// Now it's safe to cleanup this list's memory.
			Gods98::Mem_Free(list);
			m_cont.listSet[listIndex] = nullptr;
		}
		m_cont.listCount--;
	}

	/**
	 * @brief Tests if a list at the specified index is allocated and contains living items.
	 * @param listIndex The index of the list in a listSet.
	 * @return True if the list is non-null and contains at least one item that is alive.
	 */
	bool IsListAlive(size_t listIndex)
	{
		if (listIndex >= 0 && listIndex < m_cont.listCount) {
			const value_type* list = m_cont.listSet[listIndex];
			if (list) {
				const size_t count = ListSet::CountOfList(m_cont.listCount);

				for (size_t i = 0; i < count; i++) {
					if (ListSet::IsAlive(list[i]))
						return true;
				}
			}
		}
		return false;
	}


public:
	/**
	 * @brief Reduces memory usage by removing unused trailing lists.
	 * @return The number of unused lists popped from the listSet.
	 */
	size_t ShrinkToFit()
	{
		const size_t origListCount = m_cont.listCount;

		for (ptrdiff_t i = static_cast<ptrdiff_t>(origListCount) - 1; i >= 0; i--) {
			if (!this->IsListAlive(static_cast<size_t>(i))) {
				this->PopList();
			}
		}
		return (origListCount - m_cont.listCount);
	}


protected:
	size_t m_aliveCount; // Fast tracking for number of items loaned out, without having to recount.
};

#pragma endregion

}
