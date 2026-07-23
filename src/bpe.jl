module bpe 
include("tokenizer.jl")

errln(err::String) = printstyled(err; color = :red, bold = true)
warningln(warning::String) = printstyled(warning; color = :orange, bold = true)

function print_help(subcommand)
    io = stdout
    if subcommand == ""
        println(
        io, """ 
            -m, --model
                The path to the BPE model file. If no path is given the model is saved to
                `model.bpe`
            train   - Reads a text and outputs the BPE model.
            encode  - Converts text into an array of tokens using a trained model.
            decode  - Converts an array of tokens to text. 
        """)
    elseif subcommand == "train"
        println(
        io, """
            The program outputs the trained model to the file specified by the global --model flag.
            -i, --input
                The path to the text file the model will be trained on.
            -n, --iters
                The maximum number of merge iterations. Default maximum is 10000 merges.
        """
        )
    elseif subcommand == "encode"
        println(
        io, """
            -i, --input
                The path to the text file to encode.
            -o, --output
                Where to save the encoded tokens. 
        """
        )
    elseif subcommand == "decode"
        println(
        io, """
            -i, --input
                The path to the file containing the tokens in binary format.
            -o, --output
                Where to save the decoded text.
        """
        )
    else
        errln("Invalid Arguments")
    end
end

function is_valid_path(path::String)
    directory_path = path[1] == "/" ? "/" : ""
    split_path = split(path, "/")
    
    if length(split_path) == 1
        return true
    end

    while length(split_path) > 1
        x = popfirst!(split_path)
        directory_path *= x*"/"
    end

    return isdir(directory_path)
end

function (@main)(argv::Vector{String})
    input_file = ""
    output_file = ""
    model_file = "model.bpe"
    subcommand = ""
    n_iter = 10000

    while length(argv) > 0
        x = popfirst!(argv)
        if x == "-h" || x == "--help"
            print_help(subcommand)
        elseif x == "-i" || x == "--input"
            if length(argv) > 0
                input_file = popfirst!(argv)
            else
                errln("No input file path given.")
                return 1
            end
        elseif x == "-o" || x == "--output"
            if length(argv) > 0
                output_file = popfirst!(argv)
            else
                errln("No output file path given.")
                return 1
            end
        elseif x == "-n" || "iters"
            if length(argv) > 0
                try 
                    n_iter = parse(Int, popfirst!(argv))
                catch
                    errln("Iterations (-n) must be an integer,")
                    return 1
                end
            else
                warningln("No maximum number of iterations given. Default of 10000 will be used.")
            end
        elseif x == "-m" || "--model"
            if length(argv) > 0
                model_file = popfirst!(argv)
            else
                warningln("No model file path given. Default of `model.bpe` will be used.")
            end
        elseif x == "train" || x == "encode" || x == "decode"
            subcommand = x
        else
            errln("Invalid Arguments")
            return 1
        end
    end

    if !is_valid_path(model_file)
        errln("Invalid model file path.")
        return 1
    end

    try
        if subcommand == ""
            errln("No Subcommand Provided")
            return 1
        elseif subcommand == "train"
            if input_file == "" || !is_valid_path(input_file)
                errln("Invalid input file path.")
                return 1
            else
                load_file(input_file, n_iter, model_file)
            end
        elseif subcommand == "encode"
            if input_file == "" || !is_valid_path(input_file)
                errln("Invalid input file path.")
                return 1
            elseif output_file == "" || !is_valid_path(output_file)
                errln("Invalid output file path.")
                return 1
            else
                encode_file(input_file, output_file, model_file)
            end
        elseif subcommand == "decode"
            if input_file == "" || !is_valid_path(input_file)
                errln("Invalid input file path.")
                return 1
            elseif output_file == "" || !is_valid_path(output_file)
                errln("Invalid output")
                return 1
            else
                decode_file(input_file, output_file, model_file)
            end
        else
            errln("Invalid Subcommand")
            return 1
        end
    catch e
        errln("An error occurred during execution:")
        println(sprint(showerror, e))
        return 1
    end

    return 0
end

end