#include "buffer.h"


#include <fstream>

#include "commons.h"
using namespace std;

block* buffer::get_file_block(string db_name, string tb_name, int file_type, int block_num) {

    fl->increase_age();

    file* file_c = fl->get_file(db_name, tb_name, file_type);

    if (file_c) {
        block* block_c = fl->get_block(file_c, block_num);
        if (block_c) {
            return block_c;
        }
        else {
            block* bp = get_usable_block();
            bp->set_block_num(block_num);
            bp->set_file(file_c);
            bp->read_info(path);
            fl->add_block(bp);
            return bp;
        }
    }
    else {
        block* bp = get_usable_block();
        bp->set_block_num(block_num);
        file* fp = new file(db_name, file_type, tb_name, 0, 0, NULL, NULL);
        fl->add_file(fp);
        bp->set_file(fp);
        bp->read_info(path);
        fl->add_block(bp);
        return bp;
    }
    return 0;
}

block* buffer::get_usable_block() {
    if (bl -> bcount() > 0) {
        return bl->get_usable_block();
    }
    else {
        return fl->recycle_block();
    }
}

void buffer::write_block(block* block_in) { block_in->set_dirty(true); }

void buffer::write_to_disk() { std::cout << "BUFFER WRITE\n"; fl->write_to_disk(); }