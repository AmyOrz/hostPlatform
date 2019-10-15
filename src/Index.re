module IndexStoreProvider = {
  let make = Reductive.Provider.createMake(IndexStore.store);
};

ReactDOMRe.renderToElementWithId(
  <ReasonApollo.Provider client=ClientConfig.instance>
    <IndexStoreProvider component=App.make />
  </ReasonApollo.Provider>,
  "index",
);