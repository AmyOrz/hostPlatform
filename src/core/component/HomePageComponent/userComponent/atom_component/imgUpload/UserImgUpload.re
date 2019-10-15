let component = ReasonReact.statelessComponent("UserImgUpload");

let make = (~picturePath, ~imgUploadFunc, _children) => {
  ...component,
  render: _self =>
    <div className="wonder-imgUpload">
      <img src=picturePath className="imgUpload-img" />
      <div className="imgUpload-content">
        <img src="/img/editor.png" />
        <input type_="file" multiple=false onChange=imgUploadFunc />
      </div>
    </div>,
};