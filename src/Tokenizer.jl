module Tokenizer
    export load_file, decode, decode_file, encode, encode_file 
    using Printf

    # === Types and Constants ===
    const Token_t = UInt32::DataType
    const TokenPair = Tuple{Token_t, Token_t}
    
    # <|bos|> = 256, <|eos|> = 257
    const SPECIAL_TOKENS = ["<|bos|>", "<|eos|>"]
    is_byte(token) = token < 256
    is_special(token) = token in [256, 257]

    struct BPE
        pairs::Vector{TokenPair}
        merge_seq::Dict{TokenPair, Int64}
    end

    # === Constructors and File IO
    function BPE(filename::String)
        pairs = TokenPair[] 
        merge_seq = Dict{TokenPair, Int64}()
        if !isfile(filename) || filesize(filename) == 0
            append!(pairs, [(i, 0) for i in 0:257])
        else
            open(filename, "r") do io
                n_pairs = read(io, Int64)
                n_merge_seq = read(io, Int64)

                temp = Vector{TokenPair}(undef, n_pairs)
                read!(io, temp)
                append!(pairs, temp)

                for _ in 1:n_merge_seq
                    k1 = read(io, Token_t)
                    k2 = read(io, Token_t)
                    v = read(io, Int64)
                    merge_seq[(k1, k2)] = v
                end
            end
        end
        return BPE(pairs, merge_seq)
    end

    function write_bpe_file(filename::String, bpe::BPE)
        n_pairs = length(bpe.pairs)
        n_merge_seq = length(bpe.merge_seq)
        open(filename, "w") do io
            write(io, n_pairs)
            write(io, n_merge_seq)
            
            write(io, bpe.pairs)
            for(k, v) in bpe.merge_seq
                write(io, k[1])
                write(io, k[2])
                write(io, v)
            end
        end
    end

    function load_tokens(filename::String)
        n = div(filesize(filename), sizeof(Token_t))
        temp = Vector{Token_t}(undef, n)
        read!(filename, temp)
        return temp
    end
 

    # === Token Utility === 
    function render_tokens(tokens::Vector{Token_t})
        for token in tokens
            if is_byte(token)
                Printf.@printf "%c" Char(token)
            elseif token == 256
                Printf.@printf "<|bos|>"
            elseif token == 257
                Printf.@printf "<|eos|>"
            else
                Printf.@printf "[%u]" token
            end
        end
    end

    function preprocess_tokens(tokens)
        new_tokens = Token_t[]

        if !isempty(tokens)
            push!(new_tokens, 256)
        end

        for i in eachindex(tokens) 
            token = tokens[i]
            push!(new_tokens, token)

            # End of sentence or sequence, 46 is the ASCII code for "."
            if token == 46 || i == length(tokens) 
                push!(new_tokens, 257)

                if i != length(tokens)
                    push!(new_tokens, 256)
                end
            end
        end
        return new_tokens
    end


    # === Encode, Decode and Training Algorithms and Functions ===
    function encode(text::String, bpe::BPE)
        pairs = bpe.pairs
        merge_seq = bpe.merge_seq

        tokens = preprocess_tokens(Int.(codeunits(text)))
        
        while length(tokens) >= 2
            best_rank = typemax(Int)
            best_pair = nothing

            for i in 1:(length(tokens) - 1)
                pair = (tokens[i], tokens[i+1])
                rank = get(merge_seq, pair, nothing) 
                if !isnothing(rank) && rank < best_rank 
                    best_pair = pair
                    best_rank = rank
                end
            end

            if isnothing(best_pair)
                break
            end

            new_tokens = Token_t[]
            i = 1
            while i <= length(tokens)
                if i < length(tokens) && (tokens[i], tokens[i+1]) == best_pair
                    push!(new_tokens, best_rank)
                    i += 2
                else 
                    push!(new_tokens, tokens[i])
                    i += 1
                end
            end
            tokens = new_tokens
        end
            
        return Token_t.(tokens)
    end

    function encode(text::String, bpe_file::String)
        bpe = BPE(bpe_file)
        return encode(text, bpe)
    end

    function encode_file(input_file::String, output_file::String, bpe_file::String)
        bpe = BPE(bpe_file)

        io = open(output_file, "w") 
        write(io, "")
        close(io)
        open(output_file, "a") do io
            for line in eachline(input_file)
                if isempty(strip(line))
                    continue
                end
                write(io, encode(line, bpe))  
            end
        end
    end

    function decode_token(token::Token_t, pairs::Vector{TokenPair})
        bytes = UInt8[]
        function aux(tok::Token_t)
            if is_special(tok)
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
        io = IOBuffer() 
        for token in tokens
            write(io, decode_token(token, pairs))
        end
        return String(take!(io)) 
    end

    function decode(tokens::Vector{Token_t}, bpe_file::String)
        bpe = BPE(bpe_file)
        return decode(tokens, bpe.pairs)
    end

    function decode_file(input_file::String, output_file::String,bpe_file::String) 
        decoded_text = decode(load_tokens(input_file), bpe_file)
        write(output_file, decoded_text)
    end
    
    function load_file(filename::String, max_it::Int64, bpe_file::String)
        tokens = Token_t[] 
        for line in eachline(filename)
            if isempty(strip(line))
                continue 
            end
            bytes = preprocess_tokens(Int.(codeunits(line)))
            append!(tokens, bytes)
        end
        bpe_train(tokens, max_it, bpe_file)
    end

    function bpe_train(tokens::Vector{Token_t}, max_it, bpe_file)
        tokens_in = tokens
        tokens_out = Token_t[]
        
        bpe = BPE(bpe_file)
        pairs = bpe.pairs
        merge_seq = bpe.merge_seq

        n = 0
        while n < max_it 
            freqs = Dict{TokenPair, Int64}()
            for i in 1:(length(tokens_in) - 1)
                pair::TokenPair = (tokens_in[i], tokens_in[i+1])
                freqs[pair] = get(freqs, pair, 0) + 1
            end

            max_pair = nothing
            max_freq = 0
            for (pair, freq) in freqs
                if freq > max_freq
                    max_pair = pair
                    max_freq = freq
                end
            end
            if max_freq < 2
                Printf.@printf "BPE terminated after %d iterations.\n" n
                break
            end
            append!(pairs, [max_pair])
            merge_seq[max_pair] = length(pairs) - 1 

            i = 1
            while i <= length(tokens_in)
                if i < length(tokens_in) && (tokens_in[i], tokens_in[i+1]) == max_pair
                    push!(tokens_out, length(pairs) - 1)
                    i += 2
                else
                    push!(tokens_out, tokens_in[i])
                    i += 1
                end
            end

            tokens_in = tokens_out
            tokens_out = Token_t[]
            n += 1
        end

        write_bpe_file(bpe_file, BPE(pairs, merge_seq))
    end

    include("main.jl")
end