import os
import flask

app = flask.Flask(__name__)

def stringify_list(l):
    return "\n".join(l) + "\n"

def serve_file_contents(relative_path):
    try:
        with open(f"/mnt/paperplane/{relative_path}", mode="r") as reader:
            return reader.read()
    except:
        return flask.abort(404)

def serve_file(relative_path):
    try:
        return flask.send_from_directory(
            "/mnt/paperplane/",
            filename=relative_path,
            as_attachment=True)
    except:
        return flask.abort(404)

@app.route("/")
def synopsis():
    return ("GET|POST /doc", 200)

def get_all_docs():
    return (stringify_list([
        f"/doc/{ent.name}" 
        for ent in os.scandir("/mnt/paperplane/db")
        if ent.is_dir()
    ]), 200)

def upload_doc():
    pdf = flask.request.files.get("pdf")
    if pdf:
        pdf.save("/mnt/paperplane/db/upload.pdf")
        return ("Upload successful!\n", 200)
    else:
        return flask.abort(400)

@app.route("/doc", methods=["GET", "POST"])
def handle_doc():
    return (get_all_docs()
            if flask.request.method == "GET"
            else upload_doc())

@app.route("/doc")
def get_all_docs():
    return (stringify_list([
        f"/doc/{ent.name}" 
        for ent in os.scandir("/mnt/paperplane/db")
        if ent.is_dir()
    ]), 200)

@app.route("/doc/<id>")
def get_doc_by_id(id):
    return stringify_list([
        "/tokens", 
        "/text", 
        "/pdf", 
        "/pdf-checksum", 
        "/image", 
        "/image-checksum"
    ])

@app.route("/doc/<id>/tokens")
def get_doc_tokens_by_id(id):
    return serve_file_contents(f"db/{id}/tokens")

@app.route("/doc/<id>/text")
def get_doc_text_by_id(id):
    return serve_file_contents(f"db/{id}/text")

@app.route("/doc/<id>/pdf")
def get_doc_pdf_by_id(id):
    return serve_file(f"db/{id}/pdf")

@app.route("/doc/<id>/pdf-checksum")
def get_doc_pdf_checksum_by_id(id):
    return serve_file_contents(f"db/{id}/pdf-checksum")

@app.route("/doc/<id>/image")
def get_doc_image_by_id(id):
    return serve_file(f"db/{id}/image")

@app.route("/doc/<id>/image-checksum")
def get_doc_image_checksum_by_id(id):
    return serve_file_contents(f"db/{id}/image-checksum")


app.run(debug=True, host="0.0.0.0", port=80)
