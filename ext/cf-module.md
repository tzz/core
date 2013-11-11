# CFEngine Extension Protocol
RFC version 0.0.2

## General

### Purpose And Wrapping

This RFC is intended to provide CFEngine a simple protocol to communicate with extensions.

The extension types available are: discovery (current modules), body, function, and promise.

The protocol is entirely JSON-based and spoken over a bidirectional pipe. Any
invalid JSON data will be treated as a protocol error and abort the
conversation. All protocol exchanges will be logged at VERBOSE level regardless
of their validity. Valid JSON data has the format:

    { cfe_module_protocol_version: "0.0.2", ... } 

The `cfe_module_protocol_version` key **may** be abbreviated to `cmpv`.

JSON map keys don't have to be quoted.

Any functionality is expressed with keys in the `...` part.

An invalid message (invalid JSON, invalid request, unsupported version) will
**always** cause the communication to be aborted.  Thus either party in the
conversation must be prepared for that possibility at any time and if it
happens, should abort with a message explaining why (ideally quoting the bad
message in its entirety).

The protocol is bidirectional over a socket or a pipe, so both parties must be
prepared to read and write it.

The protocol has a hard-coded timeout of 15 seconds for the extension.  If the
extension doesn't write a message (which can be just `{ cmpv: "0.0.2" }` or also
include some logging progress update) before the timeout, CFEngine will abort
the communication with a message.

### Payloads

Class and variable payloads will always be encoded in JSON and can have optional
`meta` tags.  We will use `CP` and `VP` for them, with the following subtypes:

* VP(slist): `{ name: "myslistvar", data: false, value: [ "x", "y" ], meta: [ "tag1", "tag2=value3" ] }`
* VP(string): `{ name: "mystringvar", data: false, value: "x", meta: [ "tag1", "tag2=value3" ] }`
* VP(int): `{ name: "mystringvar", data: false, value: 123, meta: [ "tag1", "tag2=value3" ] }`
* VP(real): `{ name: "mystringvar", data: false, value: 123.45, meta: [ "tag1", "tag2=value3" ] }`

All the _classic_ CFEngine variable types have the `data` attribute set to `false`.

Note that the only difference between a `slist` and the other variable types
above is the value (JSON array vs. a JSON primitive).  The only difference
between an `int` and a `real` and a `string` is the type of the JSON primitive.

* VP(data): `{ name: "myvar", data: true, value: { mykey1: 123, mykey2: "value2" }, meta: [ "tag1", "tag2=value3" ] }`

To distinguish the JSON-based CFEngine data containers from the other data
types, the `data` attribute must be set to `true` or `false` and **must** be
present in any variable payload.

Class payloads can be bundle, global, or global persistent.  They all look the same:

* CP(bundle): `{ classname: "myclass", scope: "bundle", meta: [ "tag1", "tag2=value3" ] }`
* CP(global): `{ classname: "myclass", scope: "namespace", meta: [ "tag1", "tag2=value3" ] }`
* CP(persistent): `{ classname: "myclass", scope: "persist", expires: EPOCH, meta: [ "tag1", "tag2=value3" ] }`

When the scope is `persist`, it's implied that this is a global class with
persistence N minutes.  The `expires` key gives the expiration date as a Unix
epoch date.

### Extension Logging

Any time the extension puts a `log_N` key in the response, CFEngine logs that
response with `N` verbosity level. That way modules can produce VERBOSE, INFORM,
or regular output.  The value **must** be a JSON array.  For example:

    # print two INFORM messages, "hello" and "there"
    { "cfe_module_protocol_version": "0.0.2", log_INFORM: [ "hello", "there" ] } 

    # print one ERR messages, "boo"
    { "cfe_module_protocol_version": "0.0.2", log_ERR: [ "boo" ] } 

## Extension Protocol

The protocol has three stages.  The exchanges below will be shown with `CFE` to
indicate the CFEngine side (which instantiates the extension) and `EXT` to
indicate the extension side.

### Initialization

* _CFE_: `{ cmpv: "0.0.2", command: "initialize" }`
* _EXT_: four different responses are available
  * _discovery_: the following module will create variables in the `sys`
    context.

        # success must be "true"
        { cmpv: "0.0.2", success: true,
          meta: { authors: [ "Joe", "Frank" ], signature: "long hex string" },
          response: { type: "discovery", context: "sys" }
        }

  * _body_: this is an example of a body extension that can be used with the
    `classes` promise attribute.

        # success must be "true"
        { cmpv: "0.0.2", success: true,
          meta: { authors: [ "Joe", "Frank" ], signature: "long hex string" },
          response: { type: "body", promise_type: "common", promise_attribute: "classes" }
        }

  * _function_: the `return` type is one of: `string`, `real`, `int`,
    or `data`.  The `args` are all of type `data` (either a JSON
    primitive like a string or an integer or a JSON container like a
    JSON array or a JSON map).  If the **last** element of the `args`
    list is the literal `null`, then the function can take any number
    of arguments there and they will be put in a special variable
    (name TBD).  Thus the example function below takes at least 2 and
    up to any number of arguments.

        # success must be "true"
        { cmpv: "0.0.2", success: true,
          meta: { authors: [ "Joe", "Frank" ], signature: "long hex string" },
          response: { type: "function", name: "myfunction", namespace: "TBD",
                      args: [ "x", "y", null ], return: "data" }
        }

  * _promise_: this is a `twitter` promise type that must take attribute `x`, a
    string, and may take attribute `y`, a slist.

        # success must be "true"
        { cmpv: "0.0.2", success: true,
          meta: { authors: [ "Joe", "Frank" ], signature: "long hex string" },
          response: { type: "promise", name: "twitter"
                      attributes: [ { name: "x", type: "string", required: true },
                                    { name: "y", type: "slist", required: false } ] }
        }


At this point the _CFE_ side can choose to reject or accept the extension and
augment the relevant symbol tables if accepted.  The `meta` attributes may be
ignored and may, for instance, allow the extension author to sign files.

### Data exchange 

After initialization, the conversation starts with CFEngine informing the module about its state:

* _CFE_: `{ cmpv: "0.0.2", command: "state", state: DATASTATE }`

Here, `DATASTATE` is a description of the current state, as returned by the
proposed `datastate` function in https://github.com/cfengine/core/pull/1047, or
it may be a list of CP and VP data structures.  This is TBD.

The `DATASTATE` may have two formats, a "brief" (data only) and a "long" (data
plus data types and `meta` tags) output format.  This is TBD.

* _EXT_
  * _discovery_ extensions **must** reply immediately with their data, then
    **may** exit.  They **must** specify a `discovered` key in the response,
    containing a `DATASTATE` structure similar to the above.  They **may** specify
    `remove_variables` and `remove_classes` requests in the response, which the
    _CFE_ side **may** honor.

        # success must be "true"
        { cmpv: "0.0.2", success: true,
          response: { discovered: DATASTATE,
                      remove_variables: [ "badvar" ], remove_classes: [ "badclass" ] }
        }


  * _body_ extensions **must** wait for a second _CFE_ `bodycall` query with
    variable payloads indefinitely, to which they **must** reply with just a
    single response, then **may** exit.  They **must** specify a `body` key in
    the response, containing a list of variable payloads.

        # CFE second query
        { cmpv: "0.0.2", command: "bodycall", args: [ VP1, VP2, ... ] }

        # success must be "true"
        { cmpv: "0.0.2", success: true,
          response: { body: [ VP1, VP2, ... ] }
        }


  * _function_ extensions **must** wait for a second _CFE_ `funcall` query with
    variable payloads indefinitely, to which they **must** reply with just a
    single response, then **may** exit.  They **must** specify a `return` key in
    the response, containing a single variable payload.

        # CFE second query
        { cmpv: "0.0.2", command: "funcall", args: [ VP1, VP2, ... ] }

        # success must be "true"
        { cmpv: "0.0.2", success: true,
          response: { return: VP }
        }

  * _promise_ extensions **must** wait for any number of _CFE_ `verify` or
    `repair` query with variable payloads indefinitely, to which they **must**
    reply with just a single response each in the order in which they are
    received.  The first _CFE_ query **must** be a `verify` query with
    attributes.  Any attributes in the following queries are ignored.

    Attributes are encoded as VP structures.  CFEngine body attributes are
    specifically encoded as data container VPs.

    Unlike the other extensions, the `success` key may be `true`
    (repaired), `false` (not kept), or `null` (kept).

        # CFE second query
        { cmpv: "0.0.2", command: "verify", attributes: [ VP1, VP2, ... ] }

        # module says "not kept"
        { cmpv: "0.0.2", success: false }

        # CFE third query
        { cmpv: "0.0.2", command: "repair" }

        # module says "repaired"
        { cmpv: "0.0.2", success: true }

    The `verify` query should never get `true` for `success`, obviously. If it
    does, _CFE_ aborts the protocol and complains.

### Shutdown 

* _CFE_: `{ cmpv: "0.0.2", command: "shutdown" }`
* _extension_: `{ cmpv: "0.0.2", shutdown: true }`

If the exact response above is not received from the module, CFEngine complains
and treats the whole exchange as a failed promise.  Exceptions: discovery and
body extensions may exit before the `shutdown` command, in which case the
exchange is considered successful based on how it went up to that point.
