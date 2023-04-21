#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zsv.h>

struct context {
    zsv_parser parser;
};

struct fake_file {
    uint8_t *data;
    size_t size;
    size_t position;
};

void my_row_handler(void *ctx) {
    struct context *data = ctx;
    /* get a cell count */
    size_t cell_count = zsv_cell_count(data->parser);

    for (size_t i = 0; i < cell_count; i++) {
        /* use zsv_get_cell() to get our cell data */
        struct zsv_cell c = zsv_get_cell(data->parser, i);
    }
}

size_t custom_fread(void *buffer, size_t size, size_t count, struct fake_file *stream) {
    size_t readCount = count;
    if ((readCount * size) > (stream->size - stream->position)) {
        readCount = (stream->size - stream->position) / size;
    }

    memcpy(buffer, stream->data + stream->position, readCount * size);
    stream->position += readCount * size;

    return readCount;
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    struct zsv_opts opts = {0};
    struct context ctx = {0};
    struct fake_file f = {Data, Size, 0};

    opts.ctx = &ctx;
    opts.stream = &f;
    opts.read = custom_fread;

    zsv_parser parser = zsv_new(&opts);

    ctx.parser = parser;

    while (zsv_next_row(parser) == zsv_status_row) {
        size_t cell_count = zsv_cell_count(parser);

        size_t nonblank = 0;
        for (size_t i = 0; i < cell_count; i++) {
            struct zsv_cell c = zsv_get_cell(parser, i);
        }
    }

    zsv_finish(parser);
    zsv_delete(parser);
}