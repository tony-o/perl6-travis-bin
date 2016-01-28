/* Represents a bytes => chars decoding stream. */
struct MVMDecodeStream {
    /* Head and tail of the input byte buffers. */
    MVMDecodeStreamBytes *bytes_head;
    MVMDecodeStreamBytes *bytes_tail;

    /* Head and tail of the output char buffers. */
    MVMDecodeStreamChars *chars_head;
    MVMDecodeStreamChars *chars_tail;

    /* The byte position (for tell). */
    MVMint64 abs_byte_pos;

    /* How far we've eaten into the current head bytes buffer. */
    MVMint32 bytes_head_pos;

    /* How far we've eaten into the current head char buffer. */
    MVMint32 chars_head_pos;

    /* The encoding we're using. */
    MVMint32 encoding;

    /* Normalizer. */
    MVMNormalizer norm;
};

/* A single bunch of bytes added to a decode stream, with a link to the next
 * one, if any. */
struct MVMDecodeStreamBytes {
    char                 *bytes;
    MVMint32              length;
    MVMDecodeStreamBytes *next;
};

/* A bunch of characters already decoded, with a link to the next bunch. */
struct MVMDecodeStreamChars {
    MVMGrapheme32        *chars;
    MVMint32              length;
    MVMDecodeStreamChars *next;
};

/* For situations where we need to decode up to some separators, this data
 * structure holds the information about them. */
struct MVMDecodeStreamSeparators {
    /* The lengths of the separators, in graphemes. */
    MVMint32 *sep_lengths;

    /* The grapehemes themselves, in a single array (use sep_lengths to find
     * out how many there are in each separator). */
    MVMGrapheme32 *sep_graphemes;

    /* The number of separators we have. */
    MVMint32 num_seps;
};

/* Checks if we may have encountered one of the separators. This just looks to
 * see if we hit the final grapheme of any of the separators, which is all we
 * demand the actual encodings themselves work out (multi-grapheme separators
 * are handled in the decode stream logic itself). */
MVM_STATIC_INLINE MVMint32 MVM_string_decode_stream_maybe_sep(MVMThreadContext *tc, MVMDecodeStreamSeparators *sep_spec, MVMGrapheme32 g) {
    if (sep_spec) {
        MVMint32 cur_graph = -1;
        MVMint32 i;
        for (i = 0; i < sep_spec->num_seps; i++) {
            cur_graph += sep_spec->sep_lengths[i];
            if (sep_spec->sep_graphemes[cur_graph] == g)
                return 1;
        }
    }
    return 0;
}

MVMDecodeStream * MVM_string_decodestream_create(MVMThreadContext *tc, MVMint32 encoding, MVMint64 abs_byte_pos, MVMint32 translate_newlines);
void MVM_string_decodestream_add_bytes(MVMThreadContext *tc, MVMDecodeStream *ds, char *bytes, MVMint32 length);
void MVM_string_decodestream_add_chars(MVMThreadContext *tc, MVMDecodeStream *ds, MVMGrapheme32 *chars, MVMint32 length);
void MVM_string_decodestream_discard_to(MVMThreadContext *tc, MVMDecodeStream *ds, const MVMDecodeStreamBytes *bytes, MVMint32 pos);
MVMString * MVM_string_decodestream_get_chars(MVMThreadContext *tc, MVMDecodeStream *ds, MVMint32 chars);
MVMString * MVM_string_decodestream_get_until_sep(MVMThreadContext *tc, MVMDecodeStream *ds, MVMDecodeStreamSeparators *seps, MVMint32 chomp);
MVMString * MVM_string_decodestream_get_until_sep_eof(MVMThreadContext *tc, MVMDecodeStream *ds, MVMDecodeStreamSeparators *sep_spec, MVMint32 chomp);
MVMString * MVM_string_decodestream_get_all(MVMThreadContext *tc, MVMDecodeStream *ds);
MVMint64 MVM_string_decodestream_have_bytes(MVMThreadContext *tc, const MVMDecodeStream *ds, MVMint32 bytes);
MVMint64 MVM_string_decodestream_bytes_to_buf(MVMThreadContext *tc, MVMDecodeStream *ds, char **buf, MVMint32 bytes);
MVMint64 MVM_string_decodestream_tell_bytes(MVMThreadContext *tc, const MVMDecodeStream *ds);
MVMint32 MVM_string_decodestream_is_empty(MVMThreadContext *tc, MVMDecodeStream *ds);
void MVM_string_decodestream_destory(MVMThreadContext *tc, MVMDecodeStream *ds);
void MVM_string_decode_stream_sep_default(MVMThreadContext *tc, MVMDecodeStreamSeparators *sep_spec);
void MVM_string_decode_stream_sep_from_strings(MVMThreadContext *tc, MVMDecodeStreamSeparators *sep_spec, MVMString **seps, MVMint32 num_seps);
