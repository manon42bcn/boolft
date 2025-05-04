#!/bin/bash
#                   @@@@
#             @@@@@@@@@@@@@@@@    @@@@@@@@@
#            @@@@@@@@@@@@@@@@@@   @@@@@@@@@
#           ,@@@@@@@@  @@@@@@@@,  @@@@@@@@@
#           ,@@@@@@@@@.           @@@@@@@@@
#            @@@@@@@@@@@@@%       @@@@@@@@@
#              (@@@@@@@@@@@@@@    @@@@@@@@@
#                  /@@@@@@@@@@@@  ---------
#            @@@@@@@@  @@@@@@@@@  @@@@@@@@@
#            @@@@@@@@   @@@@@@@@  @@@@@@@@@@@@@@@@@@@
#            @@@@@@@@@@@@@@@@@@,  @@@@@@@@@@@@@@@@@@@@
#              @@@@@@@@@@@@@@@    @@@@@@@@@  @@@@@@@@@
#                                 @@@@@@@@@  %@@@@@@@@
# @@@@@@#             @@@@@@@@@,  @@@@@@@@@  %@@@@@@@@
# @@@@@@#             @@@@@@@@@,  @@@@@@@@@  %@@@@@@@@
# @@@@@@#.@@@@@@@&                @@@@@@@@@  %@@@@@@@@
# @@@@@@@@@@@@@@@@@/  @@@@@@@@@,  @@@@@@@@@  %@@@@@@@@  *########  #########
# @@@@@@& &@@@@@@@@%  @@@@@@@@@,  @@@@@@@@@  %@@@@@@@@  (@@@@@@@@  @@@@@@@@@
# @@@@@@#  @@@@@@@@%  @@@@@@@@@,  @@@@@@@@@  %@@@@@@@@  (@@@@@@@@  @@@@@@@@@
# @@@@@@#  @@@@@@@@%  @@@@@@@@@,  @@@@@@@@@  %@@@@@@@@  (@@@@@@@@  @@@@@@@@@
# @@@@@@#  @@@@@@@@%  @@@@@@@@@,
# @@@@@@#  @@@@@@@@%  @@@@@@@@@,
# @@@@@@#  @@@@@@@@%  @@@@@@@@@,
# @@@@@@#  @@@@@@@@%  @@@@@@@@@, BIN.sh General Tester...
# @@@@@@# %@@@@@@@@%  @@@@@@@@@,
# @@@@@@@@@@@@@@@@@   @@@@@@@@@,
# @@@@@@ /@@@@@@@(    @@@@@@@@@,
#
# whoAmI(campus=42barcelona, login=mporras-, mail=manuel.porras.ojeda@gmail.com)
# linkedin: https://www.linkedin.com/in/manuelporrasojeda
# Github: https://github.com/manon42bcn
# Feel free to write!
# Original version: https://github.com/manon42bcn/BinTester

# set to avoid expand *
set -f
# COLORS TO PRINT

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
BOLD='\033[0;1m'
NORMAL='\033[0m'

files=()
titles=()
base_dir="./Tests"
precon_errors=0
test_at_file=0
general_counter=0
report_ok=0
report_ko=0

# GET FLAGS AND OPTIONS
while getopts dlb:hsf: flag
do
    case "${flag}" in
        d) det=1;;
        l) full_logs=1;;
        b) base_dir="${OPTARG}";;
        h) help=1;;
        s) only_smoke=1;;
        f) only_file="${OPTARG}";;
        *) echo "Please run ./CppTester.sh -h for help"; exit 1;;
    esac
done

# PRINT BASIC HELP
if [[ $help ]];
then
    echo -e $BOLD$CYAN"CPP TESTER.sh HELP"$NORMAL
    echo -e "usage ./CppTester.sh"
    echo -e "flags:"
    echo -e "-s only smoke tests"
    echo -e "-d detailed output."
    echo -e "-l full logs. Default mode will log only errors."
    echo -e "-h help, basic usage help."
    echo -e "-b base directory to cases files. Default ./Tests"
    exit 0
fi

# PROGRESS BAR
show_progress() {
    local width=50
    local percent=$(( $general_counter* 100 / $test_at_file))
    local completed=$((width * $general_counter/ $test_at_file))
    local remaining=$((width - completed))

    printf "["
    printf "%${completed}s" | tr ' ' '#'
    printf "%${remaining}s" | tr ' ' '-'
    printf "] %d%% (%d/%d)\r" "$percent" "$general_counter" "$test_at_file"
}

details(){
  local result="$1"
  echo -e $BLUE"---------------------------------"$NORMAL
  echo "Test $general_counter: $title"
  echo "Entry: $input"
  echo -e "Binary: $binary"
  echo -e "$bin_execution"
  echo -e "Expected:"
  echo -e "$exp_execution"
  if [[ $result == "ok" ]] ; then
      echo -e $BOLD$GREEN"Result: OK"$NORMAL
  else
      echo -e $BOLD$RED"Result: KO"$NORMAL
  fi
}

report(){
  cases="$1"
  smoke="$2"
  warning="$3"
  total_ok=$report_ok
  total_ko=$report_ko
  sources=$(( cases + smoke ))
  total_logged=$((total_ok + total_ko))
  echo -e $BOLD$BLUE"================================================================"
  echo -e "TEST REPORT: $warning"
  echo -e "================================================================"$NORMAL
  echo "IGNORED FILES (check exec_log.txt for details): $precon_errors."
  echo "SOURCE FILES: $(( sources - precon_errors ))."
  echo "SMOKE: $smoke."
  echo "CASES: $cases."
  echo -e $BLUE"TEST LOGGED: $BOLD$total_logged."$NORMAL
  echo -e $GREEN"TOTAL PASSED TEST: $BOLD$total_ok."$NORMAL
  echo -e $RED"TOTAL FAILED TEST: $BOLD$total_ko."$NORMAL
  echo "================================================================"
  if [[ ! $full_logs && "$total_ko" == "0" ]]; then
    rm -f log.txt
  fi
  exit 0
}

check_vars_and_execute(){
  local filename="$1"
  general_counter=$((general_counter + 1))
  if ! [[ $title &&
        $binary &&
        $input &&
        $expected ]]; then
    echo "[ERROR] No minimal data founded : Malformed test file: $filename" >> exec_log.txt
    fails=$((fails + 1))
    precon_errors=$(( precon_errors + 1 ))
    report_ko=$(( report_ko + 1 ))
    # if detailed are requested, echo detailed results
    if [[ $det ]];
      then
          echo -e $RED"KO. Due to malformed test. Not minimal data founded"$NORMAL
      else
          show_progress
      fi
  else
    case_execution
  fi
}

clean_vars(){
  title=""
  binary=""
  out_fd=""
  input_type=""
  input=""
  expected_type=""
  expected_mode=false
  expected=""
}

# EXECUTE EACH TEST CASE
case_execution()
{
  local bin_execution=""
  local exp_execution=""
  # CONTROL OUTPUT FD TO CHECK
  if [[ ! $out_fd || "$out_fd" == "1" ]]; then
    # Only capture stdout
    out_fd="2>/dev/null"
  elif [[ "$out_fd" == "2" ]]; then
    # Only capture stderr
    out_fd="2>&1>/dev/null"
  elif [[ "$out_fd" == "3" ]]; then
    # stdout and stderr combined
    out_fd="2>&1"
  else
    echo -e $RED"FATAL ERROR FD [$out_fd] not recognized. (1,2,3 or none for default 1 are expected)"$NORMAL
    exit 1
  fi
  # CONTROL INPUT AS BIN EXEC
  if [[ "$input" == "[BIN]"* ]]; then
    input="${input#\[BIN\]}"
    input="$(eval $input)"
  else
    input="${input//\[EMPTY\]/ }"
  fi
  # EXPECTED AS BINARY SHOULD BE EXECUTED
  if [[ $expected == "[BIN]"* ]]; then
    expected="${expected#\[BIN\]}"
    exp_execution="$(eval $expected $input $out_fd)"
  else
    exp_execution=$expected
  fi

  # REGEX LABEL
  regex=0
  if [[ $exp_execution == "[REGEX]"* ]]; then
    regex=1
    exp_execution="${exp_execution#\[REGEX\]}"
  fi
  exp_execution=${exp_execution//\[EMPTY\]/ }

  # GET BINARY RESULTS
  bin_execution="$(eval $binary $input $out_fd)"

  # EVALUATE RESULT
  if [[ $regex == 1 ]]; then
      if echo "$bin_execution" | grep -E "$exp_execution" > /dev/null; then
        diff_output=""
      else
        diff_output="KO: exec: $bin_execution\n regex: $exp_execution"
      fi
  else
      diff_output=$(diff <(echo -e "$exp_execution") <(echo -e "$bin_execution"))
  fi

  if [[ -z "$diff_output" ]]; then
    report_ok=$((report_ok + 1))
    success=$(( success + 1 ))
    details_to_log="ok"
  else
    report_ko=$((report_ko + 1))
    fails=$(( fails + 1 ))
    details_to_log="ko"
  fi
  # Get detailed info after execution
  result=$(details $details_to_log)
  # if detailed are requested, echo detailed results
  if [[ $det ]];
  then
      echo "$result"
  else
      show_progress
  fi
  # if test fail or full logs are requested include expected and result to log
  if [[ $full_logs || $details_to_log == "ko" ]] ; then
      {
        echo -e "$result"
        echo -e "DIFF:\n" $diff_output
      } >> log.txt
  fi
}

# LOAD DATA FROM FILE
load_and_run()
{
  local filename="$1"
  success=0
  fails=0
  clean_vars
  general_counter=0
  while true; do
      IFS= read -r feature || break
      if [[ $feature == "FEATURE>>>"* ]]; then
        echo "TEST: ${feature#FEATURE>>>}"
        echo "SOURCE FILE: "$filename
      else
        echo "[ERROR] No feature title : Malformed test file: $filename" >> exec_log.txt
        precon_errors=$(( precon_errors + 1 ))
        break
      fi
      while IFS= read -r line || break 2; do
        if [[ "$line" == "#"* ]]; then
          # Commented line
          continue
        fi
        if [[ "$line" == "<<<eof" ]]; then
          # End of tests file
          break 2
        fi
        if [[ $expected_mode == true ]]; then
          # Expected mode, to read multi line expected output
          if [[ "$line" == "<<<expected"* ]]; then
            # End of expected multiline
            expected_mode=false
          else
            if ! [[ $expected ]]; then
              expected=$line
            else
              expected+="\n$line"
            fi
          fi
          continue
        fi
        if [[ "$line" == "case>>>"* ]]; then
            # New test case, clean vars and load title
            clean_vars
            title="${line#case>>>}"
        elif [[ "$line" == "<<<eof"* ]]; then
            break 2
        elif [[ "$line" == "bin>>>"* ]]; then
            binary="${line#bin>>>}"
        elif [[ "$line" == "fd>>>"* ]]; then
            out_fd="${line#fd>>>}"
        elif [[ "$line" == "input_type>>>"* ]]; then
            input_type="${line#input_type>>>}"
        elif [[ "$line" == "input>>>"* ]]; then
            input="${line#input>>>}"
        elif [[ "$line" == "expected_type>>>"* ]]; then
            expected_type="${line#expected_type>>>}"
        elif [[ "$line" == "expected>>>"* ]]; then
            expected_mode=true
            expected=""
        elif [[ "$line" == "<<<case"* ]]; then
          check_vars_and_execute $filename
          sleep 0.2
        fi
      done < <(tail -n +2 $filename)
      break
  done < "$filename"
  echo
  echo -e "$total_tests Tests have been launched: "$BOLD$GREEN$success" OK "$NORMAL" | "$BOLD$RED$fails" KO"$NORMAL
  echo "----------------------------------------------------------------"
}

# CORE RUNNER
runner()
{
  local smoke_mode=""
  cases=0
  smoke=0
  # Get an array of filenames, separated by _smoke and _cases end name. If an specific file is executed, it will be
  # loaded as a test case no matter what.
  if [[ ! $only_file ]]; then
    cases_files=$(find "${base_dir%/ 2>/dev/null}" -name *_smoke.* -type f 2>/dev/null)
    smoke=$(find "${base_dir%/ 2>/dev/null}" -name *_smoke.* -type f 2>/dev/null | wc -w)
    cases_files+=" $(find "${base_dir%/ 2>/dev/null}" -name *_cases.* -type f 2>/dev/null)"
    cases=$(find "${base_dir%/ 2>/dev/null}" -name *_cases.* -type f 2>/dev/null | wc -w)
    if [[ $smoke > 0 ]]; then
      smoke_mode=$(true)
    else
      smoke_mode=$(false)
    fi
  else
    cases_files=$only_file
    cases=1
  fi

  # _cases execution
  for file in $cases_files; do
    # Ensure that we are able to find well formed cases to be executed.
    test_at_file=$(cat $file | grep "case>>>"* | wc -l)
    if [[ $test_at_file -gt 0 ]]; then
      load_and_run $file
    else
      echo "[ERROR] No cases founded : Malformed test file: $file" >> exec_log.txt
      precon_errors=$(( precon_errors + 1 ))
      continue
    fi
    # If smoke test were found, after its execution, we must check if any fail.
    # before execute regular cases.
    if [[ $smoke_mode == $(true) && "$file" == *"_cases."* ]]; then
      if [[ $fails -ne 0 || $only_smoke ]]; then
        report $cases $smoke "ONLY SMOKE CASES WERE EXECUTED"
      fi
      smoke_mode=$(false)
    fi
  done
#
#  # Print Report
  report $cases $smoke $only_file
}

# CLEAN PREVIOUS LOGS FILES IF EXIST
rm -f log.txt
rm -f exec_log.txt
#load_and_run $file
runner
