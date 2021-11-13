import os
import flask
import subprocess
import time
import concurrent.futures

app = flask.Flask(__name__)


def dbgprint(_):
    print(_, flush=True)


def stringify_list(l):
    return "\n".join(l) + "\n"


def serve_file_contents(path):
    try:
        with open(path, mode="r") as reader:
            return reader.read()
    except:
        return flask.abort(404)


def serve_file(path):
    try:
        return flask.send_from_directory("/", filename=path, as_attachment=True)
    except:
        return flask.abort(404)


background_worker = concurrent.futures.ProcessPoolExecutor(max_workers=1)
num_background_jobs = 0


def run_native_cmd(cmd) -> str:
    process = subprocess.Popen(
        cmd,
        shell=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE)

    (stdout, stderr) = process.communicate()

    if (process.returncode != 0) or stderr:
        raise RuntimeError(stderr.decode("utf-8"))
    else:
        return stdout.decode("utf-8")


def run_native_cmd_async(cmd, on_success, on_error):
    global num_background_jobs
    num_background_jobs += 1

    def _callback(future):
        global num_background_jobs
        num_background_jobs -= 1
        err = future.exception()
        on_error(err) if err else on_success(future.result())

    background_worker.submit(
        run_native_cmd, cmd
    ).add_done_callback(_callback)


@app.route("/")
def synopsis():
    return (stringify_list([
        "+-----------------------------------------------+--------+---------------------------+",
        "| DESCRIPTION                                   | METHOD | PATH                      |",
        "+-----------------------------------------------+--------+---------------------------+",
        "| List all available document ids               | GET    | /doc                      |",
        "| Queue the archival of a new image             | POST   | /upload (file=@image.jpg) |",
        "| Get some status information                   | GET    | /status                   |",
        "+-----------------------------------------------+--------+---------------------------+"
    ]), 200)


@ app.route("/dump")
def dump_db():
    return (run_native_cmd("/bin/pppcli /opt/paperplane/db dump;"), 200)


@ app.route("/doc")
def list_docs():
    return (stringify_list([
        path.name for path in os.scandir("/opt/paperplane/db")
        if path.is_dir()
    ]), 200)


@ app.route("/doc/<id>")
def synopsis_single_doc(_):
    return (stringify_list([
        "+-------------------------------------------------------+-------+------------------+",
        "| DESCRIPTION                                           | METOD | PATH             |",
        "+-------------------------------------------------------+-------+------------------+",
        "| Get the originally uploaded image                     | GET   | /doc/id/image    |",
        "| Get the PDF document (Converted from uploaded image)  | GET   | /doc/id/pdf      |",
        "| Get the PDF document's SHA-256 hash                   | GET   | /doc/id/pdf-hash |",
        "| Get the contained text (Detected by OCR)              | GET   | /doc/id/text     |",
        "| List the # of occurrences per unique contained token  | GET   | /doc/id/tokens   |",
        "+-------------------------------------------------------+-------+------------------+"
    ]), 200)


@ app.route("/doc/<id>/tokens")
def get_doc_tokens_by_id(id):
    return serve_file_contents(f"/opt/paperplane/db/{id}/tokens")


@ app.route("/doc/<id>/text")
def get_doc_text_by_id(id):
    return serve_file_contents(f"/opt/paperplane/db/{id}/text")


@ app.route("/doc/<id>/pdf")
def get_doc_pdf_by_id(id):
    return serve_file(f"/opt/paperplane/db/{id}/pdf")


@ app.route("/doc/<id>/pdf-hash")
def get_doc_pdf_checksum_by_id(id):
    return serve_file_contents(f"/opt/paperplane/db/{id}/pdf-hash")


@ app.route("/doc/<id>/image")
def get_doc_image_by_id(id):
    return serve_file(f"/opt/paperplane/db/{id}/image")


@app.route("/status")
def get_status():
    db_footprint = run_native_cmd("du -sh /opt/paperplane/db;").split('\t')[0]
    return (stringify_list([
        f"Background jobs:    {num_background_jobs}",
        f"Database footprint: {db_footprint}"
    ]), 200)


@ app.route("/upload", methods=["POST"])
def handle_upload():
    if "file" not in flask.request.files:
        return flask.abort(400)
    else:
        try:
            image = flask.request.files["file"]
            image_path = f"/opt/paperplane/db/{image.filename}"
            image.save(image_path)
            run_native_cmd_async(
                f"/bin/sh /bin/scripts/archive.sh {image_path};"
                f"rm -f {image_path};",
                on_success=dbgprint, on_error=dbgprint)
            return (f"Processing {image.filename} in the background...\n", 200)
        except:
            return flask.abort(500)


app.run(debug=True, host="0.0.0.0", port=80)
