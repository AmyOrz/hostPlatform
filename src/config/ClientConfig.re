open ApolloInMemoryCache;

/* Create an InMemoryCache */
let inMemoryCache = createInMemoryCache();

/* Create a Link that puts an Authorization header in context */
let headerContextLink =
  ApolloLinks.createContextLink(() =>
    {
      "headers": {
        "authorization": "Bearer $123",
      },
    }
  );

/* Create an HTTP Link */
let httpLink =
  ApolloLinks.createHttpLink(~uri="https://server.wonder-3d.com/graphql", ());

/* WebSocket client */
let webSocketLink =
  ApolloLinks.webSocketLink(
    ~uri="wss://server.wonder-3d.com/graphql",
    ~reconnect=false,
    (),
  );

/* based on test, execute left or right */
let webSocketHttpLink =
  ApolloLinks.split(
    operation => {
      let operationDefition =
        ApolloUtilities.getMainDefinition(operation##query);
      operationDefition##kind == "OperationDefinition"
      &&
      operationDefition##operation == "subscription";
    },
    webSocketLink,
    httpLink,
  );

let instance =
  ReasonApollo.createApolloClient(
    ~link=ApolloLinks.from([|webSocketHttpLink, headerContextLink|]),
    ~cache=inMemoryCache,
    (),
  );