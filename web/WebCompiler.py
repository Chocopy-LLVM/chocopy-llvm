import http.server
import socketserver
import json
import sys, subprocess, os, platform

classpath_separator = ';' if platform.system() == 'Windows' else ':'

def read_file(fn):
    with open(fn,'r') as f:
           return f.read()


def write_file(fn, x):
    with open(fn,'w') as f:
           return f.write(x)

def file_exists(fn):
    return os.path.exists(fn)

class WebCompiler(http.server.SimpleHTTPRequestHandler):
    def do_POST(self):
        if self.path == "/compile":
            content_len = int(self.headers.get('Content-Length', 0))
            try:
                request_json = self.rfile.read(content_len)
                print(request_json)
                # Process request
                try:
                    request = json.loads(request_json)
                    code = request["input"]
                    passes = request["passes"]
                    code_file = ".tmp.py"
                    result_file = ".tmp.py.out"


                except Exception as e:
                    self.send_response(400)
                    self.send_header("Content-type", "text/plain")
                    self.end_headers()
                    self.wfile.write(str(e).encode('utf-8'))
                    return

                # Verify that JARs exist
                ref_jar =  "./chocopy-ref.jar"
                classpath = ref_jar

                if file_exists(ref_jar):
                    fat_jar = "./target/assignment.jar"
                    classpath = classpath + classpath_separator + fat_jar
                else:
                    fat_jar = "./target/chocopy-ref.jar"
                    classpath = fat_jar
                
                if not file_exists(fat_jar):
                    raise Exception("Could not find file %s or %s" % (ref_jar, fat_jar))



                # Compile
                write_file(code_file, code)

                stat = subprocess.run(["java", "-cp", classpath,
                    "chocopy.ChocoPy", "--pass", passes, 
                    code_file, "--json", "--out",  result_file], capture_output=True)

                if stat.returncode not in [0, 2]:
                    raise Exception(stat.stderr.decode('utf-8'))

                result_json = read_file(result_file)

                # Clean up temps
                os.remove(code_file)
                os.remove(result_file)
                print(result_json)
                # Send response
                self.send_response(200)
                self.send_header("Content-type", "application/json")
                self.end_headers()
                self.wfile.write(str(result_json).encode('utf-8'))

            except Exception as e:
                self.send_response(500)
                self.send_header("Content-type", "text/plain")
                self.end_headers()
                self.wfile.write(str(e).encode('utf-8'))
                return
        

        else:
            self.send_response(404)
            self.end_headers()
            return


if __name__ == '__main__':
    PORT = int(sys.argv[1]) if len(sys.argv) > 1 else 8000

    Handler = WebCompiler

    with socketserver.TCPServer(("", PORT), Handler) as httpd:
        print("serving at port", PORT)
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            pass
