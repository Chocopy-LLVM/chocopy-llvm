#!/usr/bin/env python3
import subprocess
import os
import argparse
import re
import time
import glob
import json5
try:
    from tqdm import tqdm
except Exception as _:
    os.system("apt install -y python3-tqdm || python3 -m pip install tqdm")
    from tqdm import tqdm
repeated_time = 3
def init_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--ConstPropagation", '-C', action='store_true')
    parser.add_argument("--LoopInvHoist",'-L', action='store_true')
    parser.add_argument("--ActiveVars",'-A', action='store_true')
    args = parser.parse_args()
    return args

def get_raw_testcases(root_path,case):
    file_names = glob.glob(root_path+'/'+case+'*.py')
    pattern=r'[0-9]+'
    file_names.sort(key= lambda item:int(re.findall(pattern, os.path.basename(item))[0]))
    return file_names

def get_baseline_files(root_path,case):
    file_names = glob.glob(root_path+'/'+case+'*.ll')
    pattern=r'[0-9]+'
    file_names.sort(key= lambda item:int(re.findall(pattern, os.path.basename(item))[0]))
    return file_names


def compile_baseline_files(file_lists):
    print("Compiling baseline files")
    progess_bar = tqdm(total=len(file_lists),ncols=50)
    exec_files = list()
    for each in file_lists:
        exec_file,_=os.path.splitext(each)
        COMMAND = "clang -mno-relax -no-integrated-as -O0 -w --target=riscv32-unknown-elf " + each + " -o " + exec_file + ".s"
        try:
            result = subprocess.run(COMMAND, stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=True,timeout=1)
            if result.returncode == 0:
                exec_files.append(exec_file)
            else:
                exec_files.append(None)
                print(f"\nCompile {each.split('/')[-1]} \033[31;1m failed\033[0m")
        except Exception as _:
            exec_files.append(None)
            print(f"Compile {each.split('/')[-1]} \033[31;1m failed\033[0m")
        COMMAND = "riscv64-unknown-elf-gcc -mabi=ilp32 -g -ggdb -static -march=rv32imac " + exec_file + ".s -o " + exec_file + " -L. -lchocopy_stdlib"
        try:
            result = subprocess.run(COMMAND, stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=True,timeout=1)
            if result.returncode == 0:
                exec_files.append(exec_file)
            else:
                exec_files.append(None)
                print(f"\nCompile {each.split('/')[-1]} \033[31;1m failed\033[0m")
        except Exception as _:
            exec_files.append(None)
            print(f"Compile {each.split('/')[-1]} \033[31;1m failed\033[0m")
        
        progess_bar.update(1)
    progess_bar.close()
    return exec_files

def compile_testcases(file_lists,option):
    compiler = "../../../build/ir-optimizer"
    COMMAND = compiler+' '+option+' '
    exec_files = list()
    print("Compiling ",option)
    progess_bar = tqdm(total=len(file_lists),ncols=50)

    for each in file_lists:

        try:
            result = subprocess.run(COMMAND+each, stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=True,timeout=1)
            if result.returncode == 0:
                exec_file,_=os.path.splitext(each)
                exec_files.append(exec_file)
            else:
                exec_files.append(None)
                print(f"\nCompile {each.split('/')[-1]} \033[31;1m failed\033[0m")
        except Exception as _:
            exec_files.append(None)
            print(f"Compile {each.split('/')[-1]} \033[31;1m failed\033[0m")

        progess_bar.update(1)
    progess_bar.close()
    return exec_files

def evaluate(file_lists, metric_func, check_mode=True):
    result = list()
    print("Evalution ")
    progess_bar = tqdm(total=len(file_lists),ncols=50)
    for each in file_lists:
        if each==None:
            result.append(None)
            continue
        if check_if_correct(each,check_mode):
            base=0
            for _ in range(repeated_time):
                re_value = metric_func(each)
                if re_value!=None:
                    base+=re_value/repeated_time
                else:
                    base=None
                    break
            result.append(base)
        else:
            result.append(None)

        subprocess.call(["rm", "-rf", each])
        progess_bar.update(1)
    progess_bar.close()
    return result

def check_if_correct(exec_file, check_mode=True):
    if check_mode:
        try:
            result = subprocess.run(exec_file, stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=10)
            with open(exec_file+'.out', "rb") as fout:
                answer = fout.read()
                if result.stdout == answer:
                    return True
                else:
                    print(f"Execute {exec_file.split('/')[-1]} result is not correct! your output:{result.stdout}, but the answer is:{answer}")
                    return False

        except Exception as _:
            print(f"Execute {exec_file.split('/')[-1]} \033[31;1m failed\033[0m")
            return False
    else:
        return True


def get_execute_time(exec_file):
    try:
        cmdline = "taskset -c 0 "+exec_file+" 2>&1"
        start = time.time()
        result = subprocess.run(cmdline, shell=True, stdout=subprocess.PIPE,stderr=subprocess.PIPE,universal_newlines=True,timeout=10)
        elapsed = time.time() - start
        return elapsed
    except Exception as e:
        print(f"Execute {exec_file.split('/')[-1]} \033[31;1m failed\033[0m")
        return None

def table_print(testcase, before_optimization, after_optimization, baseline):
    if len(before_optimization)==len(baseline) and \
            len(before_optimization) == len(after_optimization):
        pass
    else:
        max_len=max([len(before_optimization),len(after_optimization),len(baseline)])
        if len(before_optimization) < max_len:
            before_optimization+=[None]*(max_len-len(before_optimization))
        if len(after_optimization) < max_len:
            after_optimization+=[None]*(max_len-len(after_optimization))
        if len(baseline) < max_len:
            baseline+=[None]*(max_len-len(baseline))
    print(
        '\033[33;1mtestcase',
        '\t','\033[31;1mbefore optimization\033[0m',
        '\t','\033[32;1mafter optimization\033[0m',
        '\t','\033[35;1mbaseline\033[0m'
    )
    for index,(result1,result2,result3) in enumerate(zip(before_optimization,after_optimization,baseline)):
        print(testcase[index].split('/')[-1], \
              '\t\t%.2f'%result1 if result1!=None else '\t\tNone', \
              '\t\t\t%.2f'%result2 if result2!=None else '\t\t\tNone', \
              '\t\t  %.2f'%result3 if result3!=None else '\t\t  None')

def calculate_bb_score(input_bb_vals, answer_bb_vals):
    # score of every bb is between [0,1]
    score = len(list(set(input_bb_vals) & set(answer_bb_vals))) # lack of val is calculated
    score = (score-(len(input_bb_vals)-score))/len(answer_bb_vals) # extra val is calculated
    if score > 0:
        return score
    return 0

def calculate_score(input_functions, answer_functions):
    # input & answer is dict from json
    # calculate score use sum(score of every bb)/total_bb
    # score of every bb is between [0,1]
    # total_bb is count of live_in & live_out

    total_bb = 0
    for ans_func in answer_functions:
        total_bb += len(ans_func['live_in'])
        total_bb += len(ans_func['live_out'])
    cal_score = 0
    for ans_func in answer_functions:
        for in_func in input_functions:
            if ans_func['function'] == in_func['function']:
                for ans_bb, ans_bb_vals in ans_func['live_in'].items():
                    for in_bb, in_bb_vals in in_func['live_in'].items():
                        if ans_bb == in_bb:
                            cal_score += calculate_bb_score(in_bb_vals, ans_bb_vals)
                        else:
                            continue
                for ans_bb, ans_bb_vals in ans_func['live_out'].items():
                    for in_bb, in_bb_vals in in_func['live_out'].items():
                        if ans_bb == in_bb:
                            cal_score += calculate_bb_score(in_bb_vals, ans_bb_vals)
                        else:
                            continue
            else:
                continue
    return cal_score/total_bb

if __name__ == "__main__":
    script_path = os.path.join(os.getcwd(),__file__)
    usr_args = init_args()

    if usr_args.ConstPropagation:
        print("="*10,"ConstPropagation","="*10)
        root_path = os.path.join(os.path.dirname(script_path),'pass/')
        testcases = get_raw_testcases(root_path=root_path,case='const_propagate')

        exec_files1 = compile_testcases(file_lists=testcases,option='')
        results1 = evaluate(file_lists=exec_files1,metric_func=get_execute_time)

        exec_files2 = compile_testcases(file_lists=testcases,option='-pass ConstPropagation')
        results2 = evaluate(file_lists=exec_files2,metric_func=get_execute_time)

        baseline_files = get_baseline_files(os.path.join(root_path,'pass/'))
        exec_files3 = compile_baseline_files(baseline_files)
        results3 = evaluate(file_lists=exec_files3,metric_func=get_execute_time,check_mode=False)
        table_print(testcase=testcases,before_optimization=results1,after_optimization=results2,baseline=results3)

    if usr_args.LoopInvHoist:
        print("="*10,"LoopInvHoist","="*10)
        root_path = os.path.join(os.path.dirname(script_path),'pass/')
        testcases = get_raw_testcases(root_path=root_path)
        exec_files1 = compile_testcases(file_lists=testcases,option='')
        results1 = evaluate(file_lists=exec_files1,metric_func=get_execute_time)

        exec_files2 = compile_testcases(file_lists=testcases,option='-pass LoopInvariant')
        results2 = evaluate(file_lists=exec_files2,metric_func=get_execute_time)

        baseline_files = get_baseline_files(os.path.join(root_path,'pass/'))
        exec_files3 = compile_baseline_files(baseline_files)
        results3 = evaluate(file_lists=exec_files3,metric_func=get_execute_time,check_mode=False)
        table_print(testcase=testcases,before_optimization=results1,after_optimization=results2,baseline=results3)

    if usr_args.ActiveVars:
        print("="*10,"ActiveVars","="*10)
        root_path = os.path.join(os.path.dirname(script_path),'pass/')
        testcases = get_raw_testcases(root_path=root_path)

        compiler = "../../build/ir-optimizer"
        option = '-pass ActiveVars'
        COMMAND = compiler+' '+option+' '
        print("Compiling ",option)
        progess_bar = tqdm(total=len(testcases),ncols=50)

        score_list = []
        i = 0
        for each in testcases:
            i += 1
            try:
                result = subprocess.run(COMMAND+each, stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=True,timeout=1)
                if result.returncode == 0:
                    exec_file,_=os.path.splitext(each)
                    print(f"\nCompile {each.split('/')[-1]} \033[32;1m success\033[0m")
                    with open('active_vars.json', "r") as load_input:
                        with open('pass/activevar'+ str(i) +'.json', "r") as load_answer:
                            print(f"generate json {each.split('/')[-1]} \033[32;1m success\033[0m")
                            # here, input is a list of dict
                            input_functions = json5.load(load_input)
                            answer_functions = json5.load(load_answer)
                            score = calculate_score(input_functions, answer_functions)
                            score_list.append(score)
                    subprocess.call(["rm", "-rf", exec_file])
                else:
                    print(f"\nnCompile {each.split('/')[-1]} \033[31;1m failed\033[0m")
            except Exception as _:
                print(f"Analyze {each.split('/')[-1]} \033[31;1m failed\033[0m")
            progess_bar.update(1)
        progess_bar.close()

        i=0
        for score in score_list:
            i+=1
            print('testcase-'+str(i)+':',score)
