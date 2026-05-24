"""
    BPE tokenizer
"""
module Tokenizer
    export load_file, decode, encode, encode_file 
    using Printf

    const Token_t = UInt32::DataType
    const TokenPair = Tuple{Token_t, Token_t}
    
    # "<|bos|>" indicates the beginning of a sentence and is token 256
    # "<|eos|>" indicates the end of a sentence and is token 257
    const SPECIAL_TOKENS = ["<|bos|>", "<|eos|>"]
    
    is_byte(token) = token < 256 
    isspecial_token(token) = token in [256, 257] 

    # adds the "<|bos|>" and "<|eos|>" tokens where necessery 
    function add_special_tokens!(bytes::Vector{Int64}, start_of_sentence)
        if start_of_sentence
            pushfirst!(bytes, 256)
        end

        for i in 1:length(bytes) 
            if bytes[i] == Int.(codeunits("."))[1]
                insert!(bytes, i + 1, 257)
                insert!(bytes, i + 2, 256)
            end
        end
    end

    function load_tokens(filename::String)
        n = div(filesize(filename), sizeof(Token_t))
        temp = Vector{Token_t}(undef, n)
        read!(filename, temp)
        return temp
    end

    function init_pairs(filename::String)
        pairs = TokenPair[]
        if !isfile(filename) || filesize(filename) == 0
            append!(pairs, [(i, 0) for i in 0:257])
        else
            n = div(filesize(filename), sizeof(TokenPair))
            temp = Vector{TokenPair}(undef, n)
            read!(filename, pairs)
            append!(pairs, temp)
        end
        return pairs
    end

    """ 
        Train BPE using file "filename".
        Encoded byte pairs are stored in binary format in "pairs_file" 
        Max_it specifies the maximum amount of pairs to encode.
    """
    function load_file(filename::String; max_it::Int64 = 10000, pairs_file = "pairs.bin")
        tokens = Token_t[] 
        for line in eachline(filename)
            if isempty(strip(line))
                continue
            end
            bytes = Int.(codeunits(line))
            add_special_tokens!(bytes, isuppercase(line[1]))
            append!(tokens, bytes)
       end
       bpe_train(tokens, max_it, pairs_file)
    end

    function encode(text::String, pairs::Vector{TokenPair})
        tokens = Int.(codeunits(text))
        add_special_tokens!(tokens, isuppercase(text[1]))
        i = 1
        while i <= length(tokens) - 1
            pair = (tokens[i], tokens[i+1])
            search = findfirst(==(pair), pairs)
            if isnothing(search) || is_byte(search) || isspecial_token(search)
                i += 1
            else
                tokens[i] = search - 1
                deleteat!(tokens, i + 1)
                i = 1
            end 
        end
        return Token_t.(tokens)
    end

    function encode(text::String, pairs_file)
        pairs = init_pairs(pairs_file) 
        return encode(text, pairs)
    end

    function encode_file(input_file, output_file, pairs_file)
        pairs = init_pairs(pairs_file)

        io = open(output_file, "w") 
        write(io, "")
        close(io)
        open(output_file, "a") do io
            for line in eachline(input_file)
                if isempty(strip(line))
                    continue
                end
                write(io, encode(line, pairs))  
            end
        end
    end

    function decode_token(token::Token_t, pairs::Vector{TokenPair})
        bytes = UInt8[]
        function aux(tok::Token_t)
            if isspecial_token(tok)
                return
            elseif is_byte(tok)
                push!(bytes, tok)
            else
                (left, right) = pairs[tok+1]
                aux(left)
                aux(right)
            end
        end
        aux(token)
        return String(bytes)
    end

    function decode(tokens::Vector{Token_t}, pairs::Vector{TokenPair})
        text = ""
        for token in tokens
            text = text * decode_token(token, pairs)
        end
        return text
    end

    function decode(tokens::Vector{Token_t}, pairs_file)
        pairs = init_pairs(pairs_file)
        return decode(tokens, pairs)
    end

    """
        Performs BPE on a sequence of tokens. 
        max_t specifies the maximum amount of pairs to encode, the algorithm may
        stop before max_it is reached if all pairs in the sequence only occur once.
        After the algorithm is complete it stores the encoded pairs in pairs_file in binary
        format.
    """
    function bpe_train(tokens::Vector{Token_t}, max_it, pairs_file)
        freqs::Dict{TokenPair, Int64} = Dict()
        tokens_in = tokens
        tokens_out = Token_t[]
        pairs =  init_pairs(pairs_file) 

        n = 0
        while n < max_it 
            for i in 1:(length(tokens_in) - 1)
                pair::TokenPair = (tokens_in[i], tokens_in[i+1])
                freqs[pair] = get(freqs, pair, 0) + 1
            end

            max_pair = (0,0)
            max_freq = 0
            for (pair, freq) in freqs
                if freq > max_freq
                    max_pair = pair
                    max_freq = freq
                end
            end
            if max_freq < 2
                Printf.@printf "BPE terminated after %d iterations." n
                break
            end
            append!(pairs, [max_pair])

            i = 1
            while i < length(tokens_in)
                if i + 1 == length(tokens_in)
                    append!(tokens_out, [tokens_in[i], tokens_in[i+1]])
                    i += 1
                else
                    pair = (tokens_in[i], tokens_in[i+1])
                    if (pair == max_pair)
                        append!(tokens_out, length(pairs) - 1)
                        i += 2
                    else
                        append!(tokens_out, tokens_in[i])
                        i += 1
                    end
                end
            end

            tokens_in = tokens_out
            tokens_out = []
            n += 1
        end

        # save the pairs
        open(pairs_file, "w") do io
            write(io, pairs)
        end
    end
end