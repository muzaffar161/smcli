# smcli completion for bash/zsh

_smcli_completions() {
    local cur_word prev_word command_word
    cur_word="${COMP_WORDS[COMP_CWORD]}"
    prev_word="${COMP_WORDS[COMP_CWORD-1]}"
    command_word="${COMP_WORDS[1]}" # The actual command being typed

    # If no command is typed yet, or it's the first word, suggest main commands
    if [ "$COMP_CWORD" -eq 1 ]; then
        COMPREPLY=($(compgen -W "show copy cp move mv import get search find help" -- "$cur_word"))
        return 0
    fi

    case "$command_word" in
        show)
            # smcli show [path] [--depth <N>] [--exclude <name>]
            if [[ "$cur_word" == -* ]]; then
                COMPREPLY=($(compgen -W "--depth --exclude" -- "$cur_word"))
            else
                _filedir
            fi
            ;;
        copy|cp|move|mv)
            # smcli copy <source> to <destination> [as <name>]
            # smcli move <source> to <destination> [as <name>]

            # Complete paths for the source argument
            if [ "$COMP_CWORD" -eq 2 ]; then
                _filedir
            # Complete 'to' keyword after source
            elif [ "$COMP_CWORD" -eq 3 ]; then
                COMPREPLY=($(compgen -W "to" -- "$cur_word"))
            # Complete paths for the destination argument after 'to'
            elif [ "$COMP_CWORD" -eq 4 ]; then
                _filedir
            # Complete 'as' keyword after destination
            elif [ "$COMP_CWORD" -eq 5 ]; then
                COMPREPLY=($(compgen -W "as" -- "$cur_word"))
            fi
            ;;
        import|get)
            # smcli import <source> [as <name>]

            # Complete paths for the source argument
            if [ "$COMP_CWORD" -eq 2 ]; then
                _filedir
            # Complete 'as' keyword after source
            elif [ "$COMP_CWORD" -eq 3 ]; then
                COMPREPLY=($(compgen -W "as" -- "$cur_word"))
            fi
            ;;
        search|find)
            # smcli search <query> [--flags]
            # No path completion for query itself, but flags can be completed
            if [ "$COMP_CWORD" -gt 2 ]; then # After query, complete flags
                COMPREPLY=($(compgen -W "-f -fl -img -vid --exact-name --depth --exclude" -- "$cur_word"))
            fi
            ;;
        help)
            # No arguments for help command
            ;;
        *)
            # Fallback for unknown commands, or if script logic misses something
            COMPREPLY=()
            ;;
    esac
    return 0
}

complete -F _smcli_completions smcli