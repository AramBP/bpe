 #!/usr/bin/env -S julia

using Pkg

if !contains(ENV["PATH"], DEPOT_PATH[1]*"/bin")
    @warn "The environment variable PATH does not contain $(DEPOT_PATH[1]*"/bin"). Consider modifying your shell startup."
    @info "Adding $(DEPOT_PATH[1])/bin to \$PATH" ENV["PATH"]
    ENV["PATH"] = "$(DEPOT_PATH[1])/bin:$(ENV["PATH"])"
end


if isnothing(Sys.which("juliac"))
    @info "Installing JuliaC"
    Pkg.Apps.add("JuliaC")
end

@info "Compiling bpe with juliac --output-exe bpe --bundle build --trim=safe --experimental ."
run(`juliac --output-exe bpe --bundle build --trim=safe --experimental .`)
@info "The size of build/bin/bpe is $(filesize("build/bin/bpe")/1024^2) MiB"