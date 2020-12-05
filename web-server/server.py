import os
import flask

app = flask.Flask(__name__)

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

@app.route("/", methods=["GET", "POST"])
@app.route("/doc", methods=["GET", "POST"])
def index():
    if flask.request.method == "GET":
        return "\n".join([
            f"/doc/{ent.name}"
            for ent in os.scandir("/mnt/paperplane/db")
                    if ent.is_dir()
        ]) + "\n"
    else:
        pdf = flask.request.files.get("pdf")
        if pdf:
            pdf.save("/mnt/paperplane/db/upload.pdf")
            return ("Upload successful!\n", 200)
        else:
            return flask.abort(400)

@app.route("/doc/<doc>")
def doc(doc):
    return "\n".join([
        "/tokens", 
        "/text", 
        "/pdf", 
        "/pdf_sha1", 
        "/image", 
        "/image_sha1"
    ]) + "\n"  

@app.route("/doc/<doc>/tokens")
def tokens(doc):
    return serve_file_contents(f"db/{doc}/TOKENS")

@app.route("/doc/<doc>/text")
def text(doc):
    return serve_file_contents(f"db/{doc}/TEXT")

@app.route("/doc/<doc>/pdf")
def pdf(doc):
    return serve_file(f"db/{doc}/PDF")

@app.route("/doc/<doc>/pdf_sha1")
def pdf_sha1(doc):
    return serve_file_contents(f"db/{doc}/PDF_SHA1")

@app.route("/doc/<doc>/image")
def image(doc):
    return serve_file(f"db/{doc}/IMAGE")

@app.route("/doc/<doc>/image_sha1")
def image_sha1(doc):
    return doc.strip().lower() + "\n"


app.run(debug=True, host="0.0.0.0", port=80)
