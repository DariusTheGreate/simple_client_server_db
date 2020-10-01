#include "block_list.h"

block* block_list::init(block* block_in)
{
	block* adder = new block(0);
	adder->set_next(block_in->get_next());
	block_in->set_next(adder);
	count++;
	if (count == length) {
		return adder;
	}
	else {
		return init(adder);
	}
}

block_list::block_list(std::string p) : head(new block(0)), length(300), count(0), path(p)
{
	init(head);
}

block_list::~block_list() {
	block* p = head;
	while (count > 0) {
		block* pn = p->get_next();
		delete p;
		p = pn;
		count--;
	}
}

block* block_list::get_usable_block() {
	if (count == 0) {
		return NULL;
	}

	block* p = head->get_next();
	head->set_next(head->get_next()->get_next());
	count--;
	p->reset_age();
	p->set_next(NULL);
	return p;
}

int block_list::bcount()
{
	return count;
}

void block_list::free_block(block* block_in) {
	if (count == 0) {
		head = block_in;
		block_in->set_next(block_in);
	}
	else {
		block_in->set_next(head->get_next());
		head->set_next(block_in);
	}
	count++;
}