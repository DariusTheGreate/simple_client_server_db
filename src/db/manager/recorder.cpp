#include "recorder.h"

void recorder::insert(execute_insert_preparer& prep) {

	output_stream* out = output_stream::GetInstance("\n");
	out->set_value_to(std::string("its inserting yo \n"));
    auto s = sizeof(block);//24
    std::cout << s << "\n";
    std::string tb_name = prep.tb_name();
    unsigned long values_size = prep.values().size();

    table* tbl = cm_->get_db(db_name_)->get_table(tb_name);

    if (tbl == NULL) {
        throw TableNotExistException();
    }

    int max_count = (4096 - 12) / (tbl->get_record_length());//?

    std::vector<key> tkey_values;
    int pk_index = -1;

    for (int i = 0; i < values_size; ++i) {
        int value_type = prep.values()[i].data_type;
        std::string value = prep.values()[i].value_;
        int length = tbl->ats()[i].length();
        key tmp(value_type, length);
        tmp.ReadValue(value.c_str());

        tkey_values.push_back(tmp);

        if (tbl->ats()[i].attr_type() == 1) {
            pk_index = i;
        }
    }

    if (pk_index != -1) {
        std::cout << "number is " << tbl->get_index_num() << std::endl;
        if (tbl->get_index_num() != 0) {

            out->append_to_value("index greater then 0\n");
        }

        else {
            int block_num = tbl->get_first_block_num();
            for (int i = 0; i < tbl->get_block_count(); ++i) {
                block* bp = get_block(tbl, block_num);

                for (int j = 0; j < bp->get_record_count(); ++j) {
                    std::vector<key> tkey_value = get_record(tbl, block_num, j);

                    if (tkey_value[pk_index] == tkey_values[pk_index]) {
                        throw PrimaryKeyConflictException();
                    }
                }

                block_num = bp->get_next_block_num();
            }
        }

        std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n";
    }


    char* content;
    int ub = tbl->get_first_block_num();    // used block
    int frb = tbl->get_first_rubbish_num(); // first rubbish block
    int lastub;
    int blocknum, offset;

    // find the block for use from the useful block
    while (ub != -1) {
        lastub = ub;
        block* bp = get_block(tbl, ub);
        if (bp->get_record_count() == max_count) {
            ub = bp->get_next_block_num();
            continue;
        }

        std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n";
        content = bp->get_content_address() + bp->get_record_count() * tbl->get_record_length();
        
        for (std::vector<key>::iterator iter = tkey_values.begin();
            iter != tkey_values.end(); ++iter) {

            std::cout << "CCCCCCCCCCCCCCCCC\n";
            memcpy(content, iter->get_key(), iter->length());

            std::cout << "AGAGAGAGAGAGAAG\n";
            content += iter->length();
        }

        bp->set_record_count(1 + bp->get_record_count());

        blocknum = ub;
        offset = bp->get_record_count() - 1;

        hdl_->write_block(bp);
        // add record to index
        if (tbl->get_index_num() != 0) {

            out->append_to_value("index greater then 0\n");
        }

        std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n";
        hdl_->write_to_disk();

        std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n";
        cm_->write_archive_file();

        return;
    }
    std::cout << "_______HERE___" << std::endl;//
    if (frb != -1) {
        block* bp = get_block(tbl, frb);
        content = bp->get_content_address();
        for (std::vector<key>::iterator iter = tkey_values.begin();
            iter != tkey_values.end(); ++iter) {
            memcpy(content, iter->get_key(), iter->length());
            content += iter->length();
        }
        bp->set_record_count(1);

        block* lastubp = get_block(tbl, lastub);
        lastubp->set_next_block_num(frb);

        tbl->set_first_rubbish_num(bp->get_next_block_num());

        bp->set_prev_block_num(lastub);
        bp->set_next_block_num(-1);

        blocknum = frb;
        offset = 0;

        hdl_->write_block(bp);
        hdl_->write_block(lastubp);

    }
    else {
        // initial or add a block
        int next_block = tbl->get_first_block_num();
        if (tbl->get_first_block_num() != -1) {
            block* upbp = get_block(tbl, tbl->get_first_block_num());
            upbp->set_prev_block_num(tbl->get_block_count());
            hdl_->write_block(upbp);
        }
        tbl->set_first_block_num(tbl->get_block_count());
        block* bp = get_block(tbl, tbl->get_first_block_num());

        bp->set_prev_block_num(-1);
        bp->set_next_block_num(next_block);
        bp->set_record_count(1);

        content = bp->get_content_address();
        for (std::vector<key>::iterator iter = tkey_values.begin();
            iter != tkey_values.end(); ++iter) {
            memcpy(content, iter->get_key(), iter->length());
            content += iter->length();
        }

        blocknum = tbl->get_block_count();
        offset = 0;

        hdl_->write_block(bp);

        tbl->IncreaseBlockCount();
    }

    // add record to index
    if (tbl->get_index_num() != 0) {
        out->append_to_value("index greatter then 0\n");
    }
    cm_->write_archive_file();
    hdl_->write_to_disk();

    std::cout << "INSERT END\n";
}

block* recorder::get_block(table* tbl, int block_num) {
    if (block_num == -1) {
        return NULL;
    }
    block* block_ = hdl_->get_file_block(db_name_, tbl->get_tb_name(), 0, block_num);
    return block_;
}

std::vector<key> recorder::get_record(table* tbl, int block_num, int offset) {
    std::vector<key> keys;
    block* bp = get_block(tbl, block_num);

    char* content = bp->get_data() + offset * tbl->get_record_length() + 12;

    for (int i = 0; i < tbl->get_attribute_num(); ++i) {
        int value_type = tbl->ats()[i].data_type();
        int length = tbl->ats()[i].length();

        key tmp(value_type, length);

        memcpy(tmp.get_key(), content, length);

        keys.push_back(tmp);

        content += length;
    }

    return keys;
}