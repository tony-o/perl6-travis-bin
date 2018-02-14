subset CompUnit::PrecompilationId of Str:D
    where { 2 < .chars < 64 && $_ ~~ /^<[A..Za..z0..9._-]>+$/ };

role CompUnit::PrecompilationStore {
    # Load the precompilation identified by the pairing of the specified
    # compiler and precompilation ID.
    method load(CompUnit::PrecompilationId $compiler-id,
                CompUnit::PrecompilationId $precomp-id)
    { ... }

    # Store the file at the specified path in the precompilation store,
    # under the given compiler ID and precompilation ID.
    method store(CompUnit::PrecompilationId $compiler-id,
                 CompUnit::PrecompilationId $precomp-id,
                 IO::Path:D $path)
    { ... }

    # Delete an individual precompilation.
    method delete(CompUnit::PrecompilationId $compiler-id,
                  CompUnit::PrecompilationId $precomp-id)
    { ... }

    # Delete all precompilations for a particular compiler.
    method delete-by-compiler(CompUnit::PrecompilationId $compiler-id)
    { ... }
}

# vim: ft=perl6 expandtab sw=4
