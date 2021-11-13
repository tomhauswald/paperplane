import os
import json
import flask
import subprocess
import time

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


def invoke_pppcli(args):
    pid = subprocess.Popen(["/bin/pppcli"] + args, stdout=subprocess.PIPE)
    pid.wait()
    return pid.stdout.read()


def invoke_archive_script(args):
    pid = subprocess.Popen(
        ["/bin/sh", "/bin/scripts/archive.sh"] + args, stdout=subprocess.PIPE)
    pid.wait()
    return pid.stdout.read()


@app.route("/")
def synopsis():
    return (stringify_list([
        "Get a list of all documents     => [ GET  ] /doc",
        "Upload a new image for archival => [ POST ] /upload (file=@xyz)"
    ]), 200)


@ app.route("/doc")
def get_all_docs():
    return (stringify_list([
        f"/doc/{ent.name}"
        for ent in os.scandir("/opt/paperplane/db")
        if ent.is_dir()
    ]), 200)


@ app.route("/doc/<id>")
def get_doc_by_id(id):
    return stringify_list([
        "/tokens",
        "/text",
        "/pdf",
        "/pdf-hash",
        "/image",
    ])


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


@ app.route("/upload", methods=["POST"])
def archive_image():
    if "file" in flask.request.files:
        image = flask.request.files["file"]
        if image:
            image_path = f"/opt/paperplane/db/{image.filename}"
            try:
                start_time = time.time()
                image.save(image_path)
                invoke_archive_script([image_path])
                os.remove(image_path)
                return (("Successfully archived %s in %0.2f sec.\n" % (
                    image.filename, time.time() - start_time)), 200)
            except:
                return flask.abort(500)
    return flask.abort(400)


app.run(debug=True, host="0.0.0.0", port=80)
