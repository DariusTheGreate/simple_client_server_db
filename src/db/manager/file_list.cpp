#include "file_list.h"

void file_list::write_to_disk()
{
    file* fp = head;
    while (fp != NULL) {
        block* bp = fp->get_first_block();
        while (bp != NULL) {
            if (bp->is_dirty()) {
                bp->write_info(path);
                bp->set_dirty(false);
            }
            bp = bp->get_next();
        }
        fp = fp->get_next();
    }
    std::cout << "writing done" << std::endl;
}

void file_list::read_from_disk() {
    file* fp = head;
    while (fp != NULL) {
        block* bp = fp->get_first_block();
        while (bp != NULL) {
            if (bp->is_dirty()) {
                bp->read_info(path);
                bp->set_dirty(false);
            }
            bp = bp->get_next();
        }
        fp = fp->get_next();
    }
    std::cout << "reading done" << std::endl;
}

file_list::~file_list()
{
	write_to_disk();
    file* fp = head;
    while (fp != NULL) {
        file* fpn = fp->get_next();
        block* bp = fp->get_first_block();
        while (bp != NULL) {
            block* bpn = bp->get_next();
            delete bp;
            bp = bpn;
        }
        delete fp;
        fp = fpn;
    }
}

void file_list::add_file(file* file_in) {
    file* p = head;

    if (p == NULL) {
        p = file_in;
    }
    else {
        while (p->get_next() != NULL) {
            p = p->get_next();
        }
        p->set_next(file_in);
    }
}

file* file_list::get_file(std::string db_name, std::string tb_name, int file_type) {
    file* p = head;
    while (p != NULL) {
        if (p->get_data_base() == db_name && p->get_file_type() == file_type && p->get_file_name() == tb_name) {
            return p;
        }
        p = p->get_next();
    }
    return NULL;
}

block* file_list::get_block(file* file, int block_pos) {
    block* p = file->get_first_block();
    while (p != NULL) {
        if (p->get_block_num() == block_pos) {
            return p;
        }
        p = p->get_next();
    }
    return NULL;
}

void file_list::add_block(block* block_in) {
    block* p = block_in->get_file()->get_first_block();
    if (p == NULL) {
        block_in->get_file()->set_first_block(block_in);
    }
    else {
        while (p->get_next() != NULL) {
            p = p->get_next();
        }
        p->set_next(block_in);
    }
    block_in->get_file()->IncreaseRecordAmount();
    block_in->get_file()->IncreaseRecordLength();
}

void file_list::increase_age() {
    file* fp = head;
    while (fp != NULL) {
        block* bp = fp->get_first_block();
        while (bp != NULL) {
            bp->increasea_age();
            bp = bp->get_next();
        }
        fp = fp->get_next();
    }
}

block* file_list::recycle_block() {

    file* fp = head;

    block* oldestbefore = NULL;
    block* oldest = fp->get_first_block();

    while (fp != NULL) {
        block* bpbefore = NULL;
        block* bp = fp->get_first_block();
        while (bp != NULL) {

            if (bp->get_age() > oldest->get_age()) {
                oldestbefore = bpbefore;
                oldest = bp;
            }
            bpbefore = bp;
            bp = bp->get_next();
        }
        fp = fp->get_next();
    }

    if (oldest->is_dirty()) {
        oldest->write_info(path);
    }

    if (oldestbefore == NULL) {
        oldest->get_file()->set_first_block(oldest->get_next());
    }
    else {
        oldestbefore->set_next(oldest->get_next());
    }

    oldest->reset_age();
    oldest->set_next(NULL);

    return oldest;
}