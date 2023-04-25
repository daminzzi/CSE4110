#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include <mysql.h>
#include <string.h>
#pragma comment(lib, "libmysql.lib")
#define MAXLINE 2048

const char* host = "localhost";
const char* user = "root";
const char* pw = "diana0806";
const char* db = "project";

int main(void) {
	 
	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result, *sql_result2;
	MYSQL_ROW sql_row, sql_row2;

	FILE* ifp = fopen("20181297.txt", "r");
	char query[MAXLINE];
	char* buffer = NULL;
	long long int size;
	int state = 0, type, subtype, flag=0, k;
	fseek(ifp, 0, SEEK_END);
	size = ftell(ifp);
	buffer = (char*)malloc(size + 1);
	memset(buffer, 0, size + 1);
	fseek(ifp, 0, SEEK_SET);
	fread(buffer, size, 1, ifp);
	
	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");
	
	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	else
	{
		printf("Connection Succeed\n");

		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}

		const char* init = strtok(buffer, ";");
		while (1) {
			if (strstr(init, "drop table")) break;
			state = 0;
			state = mysql_query(connection, init);
			if (state != 0) printf("failure %s\n", init);
			init = strtok(NULL, ";");
		}

		while (1) {
			printf("------- SELECT QUERY TYPES -------\n\n");
			printf("\t1. TYPE 1\n");
			printf("\t2. TYPE 2\n");
			printf("\t3. TYPE 3\n");
			printf("\t4. TYPE 4\n");
			printf("\t5. TYPE 5\n");
			printf("\t6. TYPE 6\n");
			printf("\t7. TYPE 7\n");
			printf("\t0. QUIT\n");
			printf("----------------------------------\n");
			printf("Select Number: ");
			scanf("%d", &type);
			getchar();

			switch (type) {
			case 1:
				char inv_comp[20], inv_num[20];
				printf("\n\n---- TYPE 1 ----\n\n");
				printf("**Enter the company to be tracked**\n");
				scanf("%s", &inv_comp);
				printf("**Enter the invoice number to be tracked.**\nex)0000-0000-0000000\n");
				scanf("%s", &inv_num);
				getchar();
				sprintf(query, "select customer_id, c_name, phone_num from customer natural join(select customer_id from order_list, online_order where(order_list.order_num = online_order.ord_num) and (inv_comp = \"%s\" and inv_num = \"%s\"))B", inv_comp, inv_num);
				state = 0;
				state = mysql_query(connection, query);
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("customer_id: %s\nname: %s\nphone: %-s\n", sql_row[0], sql_row[1], sql_row[2]);
					}
					mysql_free_result(sql_result);
				}
				else printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
				printf("----------------------------------\n");
				printf("---- Subtypes in Type 1----\n");
				printf("	1. TYPE 1-1\n");
				printf("Select Number(If you want to go main menu, enter any integer except 1): ");
				scanf("%d", &subtype);
				getchar();
				if (subtype == 1) {
					printf("\n\n---- TYPE 1-1 ----\n\n");

					sprintf(query, "select order_prd.order_num, order_prd.product_id, order_prd.quantity, online_order.address from order_prd, online_order where(order_prd.order_num = online_order.ord_num) and (order_prd.product_id = online_order.product_id)	and (inv_num = \"%s\")", inv_num);
					state = 0;
					state = mysql_query(connection, query);
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							char* ord = sql_row[0];
							char* prd = sql_row[1];
							char* n = sql_row[2];
							char* add = sql_row[3];

							sprintf(query, "insert into online_order values (\"%s\", \"%s\", \"USPS\", \"2724-6758-1488622\", \"%s\",\"started\")", ord, prd, add);
							printf("execute: %s\n", query);
							state = 0;
							state = mysql_query(connection, query);
							if (state != 0) printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
							printf("success\n");
						}
						printf("----------------------------------\n");
						mysql_free_result(sql_result);
					}
					else printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
				}
				break;
			case 2:
				printf("\n\n---- TYPE 2 ----\n\n");
				printf("Find the customer who has bought the most (by price) in the past year.\n");
				sprintf(query, "select customer_id, c_name, sp from customer natural join (select customer_id, sum(price) as sp from order_list, order_prd where(order_list.order_num = order_prd.order_num) and (YEAR(order_list.o_date) = 2021) group by customer_id order by sp desc LIMIT 1)a; ");
				state = 0;
				state = mysql_query(connection, query);
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("customer_id: %s\nname: %s\n sum: %s\n", sql_row[0], sql_row[1], sql_row[2]);
					}
					mysql_free_result(sql_result);
				}
				else printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
				printf("----------------------------------\n");
				printf("---- Subtypes in Type 2----\n");
				printf("	1. TYPE 2-1\n");
				printf("Select Number(If you want to go main menu, enter any integer except 1): ");
				scanf("%d", &subtype);
				getchar();
				if (subtype == 1) {
					printf("\n\n---- TYPE 2-1 ----\n\n");

					sprintf(query, "select customer_id from customer natural join (select customer_id, sum(price) as sp from order_list, order_prd where(order_list.order_num = order_prd.order_num) and (YEAR(order_list.o_date) = 2021) group by customer_id order by sp desc LIMIT 1)a; ");
					state = 0;
					state = mysql_query(connection, query);
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							sprintf(query, "select product_id from (select product_id, sum(quantity) from order_prd, order_list where(order_list.order_num = order_prd.order_num) and (customer_id = \"%s\") group by product_id order by sum(quantity) desc LIMIT 1)a;", sql_row[0]);
							state = 0;
							state = mysql_query(connection, query);
							if (state == 0) {
								sql_result2 = mysql_store_result(connection);
								while ((sql_row2 = mysql_fetch_row(sql_result2)) != NULL)
								{
									printf("product_id: %s\n", sql_row2[0]);
								}
								mysql_free_result(sql_result2);
								
							}
							else printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));

						}
						printf("----------------------------------\n");
						mysql_free_result(sql_result);
					}
					else printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
				}
				break;
			case 3:
				printf("\n\n---- TYPE 3 ----\n\n");
				printf("Find all products sold in the past year.\n");
				sprintf(query, "select product_id from order_list natural join order_prd where(YEAR(order_list.o_date) = 2021) group by product_id;");
				state = 0;
				state = mysql_query(connection, query);
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					sql_row = mysql_fetch_row(sql_result);
					printf("product_id: %s", sql_row[0]);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf(", %s", sql_row[0]);
					}
					printf("\n");
					mysql_free_result(sql_result);
				}
				else printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
				printf("----------------------------------\n");
				printf("---- Subtypes in Type 3----\n");
				printf("	1. TYPE 3-1\n");
				printf("	2. TYPE 3-2\n");
				printf("Select Number(If you want to go main menu, enter any integer except 1, 2): ");
				scanf("%d", &subtype);
				getchar();
				if (subtype == 1) {
					printf("\n\n---- TYPE 3-1 ----\n\n");

					printf("find the top k products by dollar-amount sold.\n");
					printf("Which K?: ");
					k = 0;
					scanf("%d", &k);
					sprintf(query, "select product_id, sum(price) as sp from order_list natural join order_prd where(YEAR(order_list.o_date) = 2021) group by product_id order by sp desc LIMIT %d;", k);
					state = 0;
					state = mysql_query(connection, query);
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						sql_row = mysql_fetch_row(sql_result);
						printf("product_id: %s(%s $)", sql_row[0], sql_row[1]);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf(", %s(%s $)", sql_row[0], sql_row[1]);
						}
						printf("\n");
						printf("----------------------------------\n");
						mysql_free_result(sql_result);
					}
				}
				else if (subtype == 2) {
					printf("\n\n---- TYPE 3-2 ----\n\n");

					printf("find the top 10%% products by dollar-amount sold.\n");
					sprintf(query, "select count(product_id) as cnt from order_list natural join order_prd where(YEAR(order_list.o_date) = 2021)");
					k = 0;
					state = 0;
					state = mysql_query(connection, query);
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						sql_row = mysql_fetch_row(sql_result);
						k = atoi(sql_row[0]);
						mysql_free_result(sql_result);
					}
					sprintf(query, "select product_id, sum(price) as sp from order_list natural join order_prd where(YEAR(order_list.o_date) = 2021) group by product_id order by sp desc LIMIT %d;", k/10);
					state = 0;
					state = mysql_query(connection, query);
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						sql_row = mysql_fetch_row(sql_result);
						printf("product_id: %s(%s $)", sql_row[0], sql_row[1]);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf(", %s(%s $)", sql_row[0], sql_row[1]);
						}
						printf("\n");
						printf("----------------------------------\n");
						mysql_free_result(sql_result);
					}
				}
				break;
			case 4:
				printf("\n\n---- TYPE 4 ----\n\n");
				printf("Find all products by unit sales in the past year.\n");
				sprintf(query, "select product_id, sum(quantity) from order_list natural join order_prd where(YEAR(order_list.o_date) = 2021) group by product_id;");
				state = 0;
				state = mysql_query(connection, query);
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					sql_row = mysql_fetch_row(sql_result);
					printf("product_id: %s*%s", sql_row[0], sql_row[1]);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf(", %s*%s", sql_row[0], sql_row[1]);
					}
					printf("\n");
					mysql_free_result(sql_result);
				}
				else printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
				printf("----------------------------------\n");
				printf("---- Subtypes in Type 4----\n");
				printf("	1. TYPE 4-1\n");
				printf("	2. TYPE 4-2\n");
				printf("Select Number(If you want to go main menu, enter any integer except 1, 2): ");
				scanf("%d", &subtype);
				getchar();
				if (subtype == 1) {
					printf("\n\n---- TYPE 4-1 ----\n\n");

					printf("find the top k products by dollar-amount sold.\n");
					printf("Which K?: ");
					k = 0;
					scanf("%d", &k);
					sprintf(query, "select product_id, sum(quantity) as sq from order_list natural join order_prd where(YEAR(order_list.o_date) = 2021) group by product_id order by sq desc LIMIT %d;", k);
					state = 0;
					state = mysql_query(connection, query);
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						sql_row = mysql_fetch_row(sql_result);
						printf("product_id: %s*%s", sql_row[0], sql_row[1]);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf(", %s*%s", sql_row[0], sql_row[1]);
						}
						printf("\n");
						printf("----------------------------------\n");
						mysql_free_result(sql_result);
					}
				}
				else if (subtype == 2) {
					printf("\n\n---- TYPE 4-2 ----\n\n");

					printf("find the top 10%% products by dollar-amount sold.\n");
					sprintf(query, "select count(product_id) as cnt from order_list natural join order_prd where(YEAR(order_list.o_date) = 2021)");
					k = 0;
					state = 0;
					state = mysql_query(connection, query);
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						sql_row = mysql_fetch_row(sql_result);
						k = atoi(sql_row[0]);
						mysql_free_result(sql_result);
					}
					sprintf(query, "select product_id, sum(quantity) as sq from order_list natural join order_prd where(YEAR(order_list.o_date) = 2021) group by product_id order by sq desc LIMIT %d;", k / 10);
					state = 0;
					state = mysql_query(connection, query);
					if (state == 0)
					{
						sql_result = mysql_store_result(connection);
						sql_row = mysql_fetch_row(sql_result);
						printf("product_id: %s*%s", sql_row[0], sql_row[1]);
						while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
						{
							printf(", %s*%s", sql_row[0], sql_row[1]);
						}
						printf("\n");
						printf("----------------------------------\n");
						mysql_free_result(sql_result);
					}
				}
				break;
			case 5:
				printf("\n\n---- TYPE 5 ----\n\n");
				printf("Find those products that are out-of-stock at every store in California.\n");
				sprintf(query, "select product_id from product where product_id not in (select product_id from inventory natural join store where store.location = \"California\" and inventory.quantity > 0);");
				state = 0;
				state = mysql_query(connection, query);
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					sql_row = mysql_fetch_row(sql_result);
					printf("product_id: %s", sql_row[0]);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf(", %s", sql_row[0]);
					}
					printf("\n");
					printf("----------------------------------\n");
					mysql_free_result(sql_result);
				}
				else printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
				break;
			case 6:
				printf("\n\n---- TYPE 6 ----\n\n");
				printf("Find those packages that were not delivered within the promised time.\n");
				sprintf(query, "with a as(select order_num, inv_num, delivery_status, o_date,  datediff(date(now()), date(o_date)) as diff from (order_list natural join order_prd), online_order where(order_list.order_num = online_order.ord_num) and (order_prd.product_id = online_order.product_id)) select inv_num, diff from a where(diff > 3) and (delivery_status != \"done\");");
				state = 0;
				state = mysql_query(connection, query);
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					sql_row = mysql_fetch_row(sql_result);
					printf("inv_num: %s(%s days)", sql_row[0], sql_row[1]);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf(", %s(%s days)", sql_row[0], sql_row[1]);
					}
					printf("\n");
					printf("----------------------------------\n");
					mysql_free_result(sql_result);
				}
				else printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
				break;
			case 7:
				printf("\n\n---- TYPE 7 ----\n\n");
				printf("Generate the bill for each customer for the past month.\n");
				sprintf(query, "select customer_id, sum(price), count(order_num) from order_prd natural join order_list where payment_status = \"x\" and MONTH(order_list.o_date) = 5 group by customer_id;");
				state = 0;
				state = mysql_query(connection, query);
				if (state == 0)
				{
					printf("customer_id|Number of orders|total price\n");
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%9s %15s %11s\n", sql_row[0], sql_row[2], sql_row[1]);
					}
					printf("----------------------------------\n");
					mysql_free_result(sql_result);
				}
				else printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
				break;
			case 0:
				flag = 1;
				break;
			default:
				printf("Enter the right option(0~7).\n");
				break;
			}
			if (flag == 1) break;
		}

		init = strtok(NULL, ";");

		while (init != NULL) {
			state = 0;
			state = mysql_query(connection, init);
			if (state != 0) printf("failure %s\n", init);
			init = strtok(NULL, ";");
		}

		mysql_close(connection);
	}

	return 0;
}
